#pragma once

#include "Packet.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <ratio>
#include <thread>
#include <vector>

// Forward declaration for PacketFinishComparator
class PacketFinishComparator;

class Scheduler {
public:
  Scheduler(int numSrc, std::vector<float> wts, int ticksize,
            double processingCapacity, int queueCapacity, int simulationTicks);

  void UpdateRoundNumber();

  void addPacket(int src, int pktlen);

  void ProcessPacket(int src, int dst);

  void StartProcessing();

  ~Scheduler();

  static std::shared_ptr<std::mutex> schedmutex;

private:
  int ticksize; // number of milliseconds in a tick
  double processingCapacity;
  int queueCapacity;
  int simulationTicks;
  int numSrc;
  double roundNumber;
  double sumActiveWeights;

  int printStats = 0;

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

  // wakes up, takes the lock, removes an element from queue, returns lock, and
  // sleeps for transmission time if queue is empty it waits for signal to wake
  // up
  static void ProcessingThread(int ticksize, int processingCapacity, int simulationTicks) {

    std::chrono::milliseconds transmissionTime;
    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(ticksize * simulationTicks);
    while (std::chrono::steady_clock::now() < endTime) {
      std::unique_lock<std::mutex> lk(*Scheduler::schedmutex);
      Scheduler::QueueNotEmpty.wait(lk, []() {
        return !queue.empty();
      }); // check if the function is correct or not

      Packet pkt = queue.top();
      queue.pop();

      pkt.deqTime = std::chrono::steady_clock::now();
      transmissionTime =
          std::chrono::milliseconds(ticksize * pkt.length / processingCapacity);
      pkt.egressTime = pkt.deqTime + transmissionTime;

      lk.unlock();
      std::this_thread::sleep_for(transmissionTime);
    }
  }

  std::thread processingThread;
};