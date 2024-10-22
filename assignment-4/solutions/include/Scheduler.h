#pragma once

#include "Packet.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <ratio>
#include <thread>
#include <vector>

class Scheduler {
public:
  struct Stats {

    struct SourceMetrics {
      int64_t total_packet_length_generated;
      int64_t total_packet_length_transmitted;
      double percentage_transmitted;
      double fraction_allocated;
      double averag_pkt_delay;
      double packet_drop_prob;
    };

    double server_utilisation;
    std::queue<double> jfi, rfb;
    double average_pkt_delay;
    double packet_drop_prob;
    std::vector<SourceMetrics> srcs;
  };

  Scheduler(int numSrc, std::vector<float> wts, int ticksize,
            double processingCapacity, int queueCapacity, int simulationTicks);

  void UpdateRoundNumber();

  void addPacket(int src, int pktlen);

  void ProcessPacket(int src, int dst);

  void StartProcessing();

  ~Scheduler();

  Stats GetStats() {
    Stats st;
    st.jfi = jains_fairness_idx;
    st.rfb = rfb;

    // average packet delay
    st.average_pkt_delay = 0;
    int64_t total_delay = 0;
    for (auto pkt_delay_sum_per_src : sum_pkt_delays)
      total_delay += pkt_delay_sum_per_src;
    int64_t total_number_packets = 0;
    for (int i = 0; i < numSrc; i++)
      total_number_packets += pkts_generated[i] - pkts_dropped[i];
    st.average_pkt_delay = ((double)total_delay) / total_number_packets;

    // average packet drop probab
    for (int i = 0; i < numSrc; i++)
      st.packet_drop_prob += pkts_dropped[i];
    st.packet_drop_prob /= total_number_packets;

    // for each source
    for (int i = 0; i < numSrc; i++) {
      Stats::SourceMetrics sm;
      sm.total_packet_length_generated = length_pkts_generated[i];
      sm.total_packet_length_transmitted =
          sm.total_packet_length_generated - dropped_pkt_length[i];
      sm.packet_drop_prob = ((double)pkts_dropped[i]) / pkts_generated[i];
      sm.averag_pkt_delay = (sum_pkt_delays[i]) / pkts_generated[i];
      sm.fraction_allocated = sm.total_packet_length_transmitted /
                              (simulationTicks * processingCapacity);
      sm.percentage_transmitted =
          static_cast<double>(sm.total_packet_length_transmitted) /
          sm.total_packet_length_generated;

      st.srcs.push_back(std::move(sm));
    }

    return st;
  }

  static std::shared_ptr<std::mutex> schedmutex;

private:
  int ticksize; // number of milliseconds in a tick
  double processingCapacity;
  int queueCapacity; // increased by packet drops and decreased on attempting to
                     // transmit a dropped packet. Basically there can be
                     // dropped packets as well in the queue
  int simulationTicks;
  int numSrc;
  double roundNumber;
  double sumActiveWeights;

  long long int printStats = 0;

  std::optional<std::chrono::steady_clock::time_point> lastTrigger;

  std::vector<int> numPackets;
  std::vector<float>
      weights; // stores the weights for every source, indexed by source number
  std::vector<double> flowFinishNumbers; // stores the finish numbers of all the
                                         // sources, indexed by src numbers

  static std::priority_queue<Packet, std::vector<Packet>,
                             PacketFinishComparator>
      queue; // stores packets in the order of finish number

  static std::condition_variable QueueNotEmpty;

  // metric Thread
  // wakes up after every `separation_index_calculations` number of ticks apart
  // calculates Jain's Fairness index and RBF and then sleeps again
  static void MetricThread(const int ticksize,
                           const int separation_index_calculations,
                           Scheduler *schedptr,
                           const std::vector<double> fairAllocation,
                           int simulationTicks) {
    std::vector<int64_t> pkt_lengths_generated;
    std::vector<int64_t> pkt_lengths_dropped;
    std::vector<double> transmitted;
    int64_t total_length_transmitted;

    std::chrono::steady_clock::time_point nextTime =
        std::chrono::steady_clock::now();
    nextTime +=
        std::chrono::milliseconds(ticksize * separation_index_calculations);
    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now() +
        std::chrono::milliseconds(simulationTicks * ticksize);

    while (std::chrono::steady_clock::now() < endTime) {
      std::this_thread::sleep_until(nextTime);
      nextTime +=
          std::chrono::milliseconds(ticksize * separation_index_calculations);

      // acquire the lock and copy te required data
      Scheduler::schedmutex->lock();
      total_length_transmitted = schedptr->total_length_transmitted;
      pkt_lengths_generated = schedptr->length_pkts_generated;
      pkt_lengths_dropped = schedptr->dropped_pkt_length;
      Scheduler::schedmutex->unlock();

      assert(pkt_lengths_dropped.size() == pkt_lengths_generated.size());
      int numSrc = pkt_lengths_dropped.size();

      for (int i = 0; i < numSrc; i++)
        std::cout << "pkt_lengths_generated[" << i << "] = " << pkt_lengths_generated[i] << "\n";

      for (int i = 0; i < numSrc; i++)
        std::cout << "pkt_lengths_dropped[" << i << "] = " << pkt_lengths_dropped[i] << "\n";

      transmitted.clear();
      for (int i=0; i < numSrc; i++)
        transmitted.push_back(pkt_lengths_generated[i] -
                              pkt_lengths_dropped[i]);

      for (int i = 0; i < numSrc; i++)
        transmitted[i] /= total_length_transmitted;

      for (int i = 0; i < numSrc; i++)
        std::cout << "transmitted[" << i << "] = " << transmitted[i] << "\n";

      double jfi = 0;
      for (auto i : transmitted)
        jfi += i;
      std::cout << "jfi1: " << jfi << "\n";
      jfi = (jfi * jfi) / transmitted.size();
      std::cout << "jfi2: " << jfi << "\n";
      double denominator = 0.0;
      for (auto i : transmitted)
        denominator += i * i;
      jfi /= denominator;
      std::cout << "jfi3: " << jfi << "\n";
      schedptr->jains_fairness_idx.push(jfi);

      for (int i = 0; i < transmitted.size(); i++)
        transmitted[i] /= fairAllocation[i];

      // now find the max over all pairs
      double rfb = 0;
      for (int i = 0; i < transmitted.size(); i++)
        for (int j = 0; j < i; j++)
          rfb = std::max(rfb, std::abs(transmitted[i] - transmitted[j]));
      schedptr->rfb.push(rfb);
    }
  }

  // wakes up, takes the lock, removes an element from queue, returns lock, and
  // sleeps for transmission time if queue is empty it waits for signal to wake
  // up
  static void ProcessingThread(const int ticksize, const int processingCapacity,
                               const int simulationTicks, Scheduler *schedptr) {

    std::chrono::milliseconds transmissionTime;
    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now() +
        std::chrono::milliseconds(ticksize * simulationTicks);
    while (std::chrono::steady_clock::now() < endTime) {

      std::unique_lock<std::mutex> lk(*Scheduler::schedmutex);

      if (queue.empty()) {
        Scheduler::QueueNotEmpty.wait_until(lk, endTime, []() {
          return !queue.empty();
        }); // check if the function is correct or not
      }

      Packet pkt = queue.top();
      queue.pop();

      if (schedptr->toBeDropped[pkt.sourceNumber] > 0) {
        schedptr->toBeDropped[pkt.sourceNumber]--;
        schedptr->queueCapacity--;
        continue;
      }

      schedptr->total_length_transmitted += pkt.length;

      pkt.deqTime = std::chrono::steady_clock::now();
      transmissionTime =
          std::chrono::milliseconds(ticksize * pkt.length / processingCapacity);
      pkt.egressTime = pkt.deqTime + transmissionTime;

      schedptr->sum_pkt_delays[pkt.sourceNumber] +=
          static_cast<double>((pkt.deqTime - pkt.enqTime).count() / 1e6) /
              ticksize +
          static_cast<double>(pkt.length) / processingCapacity; // waiting time

      lk.unlock();
      std::this_thread::sleep_for(transmissionTime);
    }
  }

  //// STATS ///

  std::vector<int>
      toBeDropped; // number of head of flow packets to be dropped for each flow

  std::vector<int64_t>
      pkts_generated; // number of packets generated for each flow
  std::vector<int64_t>
      length_pkts_generated; // number of packets generated for each flow
  std::vector<int64_t> pkts_dropped; // number of packets dropped for each flow
  std::vector<int64_t>
      dropped_pkt_length; // sum of lengths of all dropped packets for each flow

  std::vector<double> sum_pkt_delays;

  int64_t total_length_transmitted;
  std::vector<double> fair_allocation; // as fractions
  std::queue<double> jains_fairness_idx, rfb;
  const int separation_index_calculations = 100;

  //////////////

  std::chrono::steady_clock::time_point threadEndTime;
  std::thread processingThread;

  ////////////////

  std::thread metricThread;
};