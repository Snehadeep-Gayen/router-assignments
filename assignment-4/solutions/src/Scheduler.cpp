
#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <thread>

#include "Scheduler.h"
#include "Simulator.h"

#define STEPSIZE (10000ll)

std::shared_ptr<std::mutex> Scheduler::schedmutex =
    std::make_shared<std::mutex>();

std::priority_queue<Packet, std::vector<Packet>, PacketFinishComparator>
    Scheduler::queue = {};

std::condition_variable Scheduler::QueueNotEmpty = {};

Scheduler::Scheduler(int numSrc, std::vector<float> wts, int ticksize,
                     double processingCapacity, int queueCapacity,
                     int simulationTicks)
    : processingCapacity(processingCapacity), ticksize(ticksize),
      queueCapacity(queueCapacity), simulationTicks(simulationTicks),
      numSrc(numSrc), numPackets(numSrc, 0), weights(wts),
      flowFinishNumbers(numSrc, -1), toBeDropped(numSrc, 0),
      pkts_generated(numSrc, 0), pkts_transmitted(numSrc, 0),
      length_pkts_generated(numSrc, 0), length_pkts_transmitted(numSrc, 0),
      pkts_dropped(numSrc, 0), dropped_pkt_length(numSrc, 0),
      sum_pkt_delays(numSrc, 0), total_length_transmitted(0) {

  roundNumber = 0.0;
  sumActiveWeights = 0.0;
  lastTrigger = std::nullopt;

  // calculate fair allocation
  double sum_weights = 0;
  std::cout << "Weights are : ";
  for (auto wt : wts) {
    std::cout << wt << " ";
    sum_weights += wt;
    fair_allocation.push_back(wt);
  }
  std::cout << "\n";
  for (auto &wt : fair_allocation)
    wt /= sum_weights;
}

void Scheduler::StartProcessing() {
  processingThread = std::thread(ProcessingThread, ticksize, processingCapacity,
                                 simulationTicks, this);
  threadEndTime = std::chrono::steady_clock::now() +
                  std::chrono::milliseconds(simulationTicks * ticksize +
                                            BUFFER_TIME * 1000);

  // start the metric thread also
  metricThread =
      std::thread(MetricThread, ticksize, separation_index_calculations, this,
                  fair_allocation, simulationTicks);
}

void Scheduler::UpdateRoundNumber() {
  if (lastTrigger.has_value()) {
    bool all_inactive = std::all_of(
        flowFinishNumbers.begin(), flowFinishNumbers.end(),
        [this](double finishNumber) { return finishNumber <= roundNumber; });
    if (all_inactive) {
      roundNumber = 0;
      if (std::abs(sumActiveWeights) > 1e-3) {
        std::cout << sumActiveWeights << "\n";
        exit(0);
      }
      sumActiveWeights = 0.0;
      flowFinishNumbers = std::vector<double>(numSrc, -1);
      return;
    }

    const auto &timenow = std::chrono::steady_clock::now();
    double projectedRoundNumber;
    const auto &UpdateProjectedRoundNumber = [&projectedRoundNumber, &timenow,
                                              this]() {
      double durationInMillis =
          std::chrono::duration_cast<std::chrono::duration<double>>(
              timenow - lastTrigger.value())
              .count() *
          1000 / ticksize;
      if (printStats == STEPSIZE)
        std::cout << "Duration is " << durationInMillis << "\n";
      projectedRoundNumber =
          roundNumber + durationInMillis / (std::abs(sumActiveWeights) < 1e-3
                                                ? (INFINITY)
                                                : sumActiveWeights);
    };
    UpdateProjectedRoundNumber();

    double sumWeights = 0.0;
    std::vector<std::pair<double, double>> sortedFlows;
    for (int src = 0; src < numSrc; src++)
      if (flowFinishNumbers[src] >= roundNumber) {
        sumWeights += weights[src];
        sortedFlows.push_back({flowFinishNumbers[src], weights[src]});
      }
    std::sort(sortedFlows.begin(), sortedFlows.end());
    if (std::abs(sumWeights - sumActiveWeights) > 1e-3) {
      std::cout << "Difference in weights\n";
      std::cout << sumActiveWeights << " " << sumWeights << "\n";
      exit(0);
    }

    double startingRoundNumber = roundNumber;
    for (auto [finishNumber, weight] : sortedFlows) {
      if (finishNumber < projectedRoundNumber) {
        std::chrono::microseconds microTime(
            static_cast<int64_t>(((finishNumber - roundNumber) *
                                  sumActiveWeights * ticksize * 1000)));
        std::chrono::steady_clock::time_point inactiveTime =
            lastTrigger.value() + microTime;
        roundNumber = finishNumber;
        lastTrigger = inactiveTime;
        sumActiveWeights -= weight;

        assert(sumActiveWeights >= -1e-3);
      } else {
        break;
      }
    }

    roundNumber = projectedRoundNumber;
    lastTrigger = timenow;
  } else {
    roundNumber = 0.0;
    lastTrigger = std::chrono::steady_clock::now();
  }
}

void Scheduler::addPacket(int src, int pktlen) {
  numPackets[src]++;

  if (++printStats == STEPSIZE) {
    std::cout << "Round# =" << roundNumber << "\n";
    std::cout << "SumWeights =" << sumActiveWeights << "\n";
    for (int i = 0; i < numSrc; i++) {
      std::cout << "Finish Number for source#" << i << " is "
                << flowFinishNumbers[i] << "\n";
    }
    printStats = 0;
  }
  UpdateRoundNumber();

  //// update the stats ////

  pkts_generated[src]++;
  length_pkts_generated[src] += pktlen;

  //////////////////////////

  Packet pkt{.sourceNumber = src,
             .length = pktlen,
             .enqTime = std::chrono::steady_clock::now()};

  pkt.finishNumber = std::max(roundNumber, flowFinishNumbers[src]) +
                     pkt.length / (processingCapacity * weights[src]);

  if (flowFinishNumbers[src] < roundNumber) {
    sumActiveWeights += weights[src];
  }

  flowFinishNumbers[src] = std::max(flowFinishNumbers[src], pkt.finishNumber);

  if (queue.size() < queueCapacity) {
    Scheduler::queue.push(std::move(pkt));
  } else {
    pkts_dropped[src]++;
    dropped_pkt_length[src] += pktlen;
    
    queueCapacity++;
    toBeDropped[src]++;
    Scheduler::queue.push(std::move(pkt));
  }

  Scheduler::QueueNotEmpty.notify_one();
}

Scheduler::~Scheduler() {
  for (int i = 0; i < numSrc; i++) {
    std::cout << numPackets[i] << " packets came from source #" << i << "\n";
  }

  if (processingThread.joinable()) {
    std::this_thread::sleep_until(threadEndTime);
    QueueNotEmpty.notify_all();
    if (processingThread.joinable())
      processingThread.join();
  }
}
