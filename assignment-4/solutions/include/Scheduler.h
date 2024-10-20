#pragma once

#include "Packet.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <optional>
#include <queue>
#include <vector>

// Forward declaration for PacketFinishComparator
class PacketFinishComparator;

class Scheduler {
public:
  Scheduler(int numSrc, std::vector<float> wts, int ticksize,
            double processingCapacity);

  void UpdateRoundNumber();

  void addPacket(int src, int pktlen);

  ~Scheduler();

private:
  int ticksize; // number of milliseconds in a tick
  double processingCapacity;
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

  std::priority_queue<Packet, std::vector<Packet>, PacketFinishComparator>
      queue; // stores packets in the order of finish number
};