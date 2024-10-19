#include <iostream>
#include <vector>
#include "TrafficGenerator.hpp"
#include <cassert>

int main()
{
  // // test that the traffic generator approximately generates the correct number of packets
  // std::vector<long double> prob = {0.1, 0.3, 0.6, 0.8786};
  // std::vector<int> numPorts = {2, 4, 8, 16, 30};

  // for(auto p : prob)
  //   for(auto n : numPorts)
  //   {
  //     int iter = 100000;
  //     TrafficGenerator tg(n, (float) p);
  //     std::vector<long long> portSum(n, 0);
  //     long long int numPackets = 0;
  //     for(int i=0; i<iter; i++)
  //     {
  //       for(auto pkt_opt : tg.GeneratePackets(i))
  //       {
  //         if(pkt_opt.has_value())
  //         {
  //           numPackets++;
  //           portSum[pkt_opt->outputPort]++;
  //         }
  //       }
  //     }

  //     // check if the stats approximately match
  //     if(std::abs(numPackets / (p * iter * n) - 1) > 0.05f)
  //     {
  //       std::cout << "numPackets: " << numPackets << std::endl;
  //       std::cout << "p: " << p << std::endl;
  //       std::cout << "iter: " << iter << std::endl;
  //       std::cout << "n: " << n << std::endl;
  //       std::cout << "Expression value: " << std::abs(numPackets / (p * iter * n)) << std::endl;
  //       exit(0);
  //     }

  //     for(int i=0; i<n; i++)
  //     {
  //       if(std::abs(portSum[i] / (p * iter) - 1) > 0.05)
  //       {
  //           std::cout << "portSum[" << i << "]: " << portSum[i] << std::endl;
  //           std::cout << "portSum[" << i << "] / (p * iter): " 
  //                     << portSum[i] / (p * iter) << std::endl;
  //           std::cout << "p: " << p << std::endl;
  //           std::cout << "iter: " << iter << std::endl;
  //           exit(0);
  //       }
  //     }
  //   }
  return 0;
}
