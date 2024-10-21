
#include <algorithm>
#include <iostream>
#include <set>

#include "Scheduler.h"

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
      flowFinishNumbers(numSrc, -1) {
  roundNumber = 0.0;
  sumActiveWeights = 0.0;
  lastTrigger = std::nullopt;
}

void Scheduler::StartProcessing() {
  processingThread =
      std::thread(ProcessingThread, ticksize, processingCapacity, simulationTicks);
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
      if (printStats == 0)
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
      if (flowFinishNumbers[src] > roundNumber) {
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

  if (++printStats == 1) {
    std::cout << "Round# =" << roundNumber << "\n";
    std::cout << "SumWeights =" << sumActiveWeights << "\n";
    for (int i = 0; i < numSrc; i++) {
      std::cout << "Finish Number for source#" << i << " is "
                << flowFinishNumbers[i] << "\n";
    }
    printStats = 0;
  }
  UpdateRoundNumber();

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
    ; // Packet dropped here
  }

  Scheduler::QueueNotEmpty.notify_one();
}

Scheduler::~Scheduler() {
  for (int i = 0; i < numSrc; i++) {
    std::cout << numPackets[i] << " packets came from source #" << i << "\n";
  }
  if(processingThread.joinable())
    processingThread.join();
}
