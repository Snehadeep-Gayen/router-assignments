#pragma once

#include <cassert>
#include <chrono>
#include <memory>
#include <ostream>
#include <string>
#include <fstream>
#include "Logging.h"
#include <thread>
#include <vector>
#include "PacketGen.h"
#include "Scheduler.h"

namespace Simulation{

#define SIMULATOR_LOGMODULE "Simulator"
#define SETUP_TIME (2)
#define BUFFER_TIME (2)
// number of milliseconds in one tick
#define TICKSIZE (100)

    struct Config
    {
        int n;  // number of sources
        int t;  // total simulation time
        int c;  // pkt processing capacity of the output link
        int b;  // queue capacity
        std::vector<PacketGen::PacketSourceConfig> pktSrcConfigs;   // configuration of each packet
        std::vector<float> weights; // weights of each packet source
        std::string outputfilename; // output file to write to
    };

    class Simulator
    {
        public:
            Simulator(struct Config&& conf) : conf(conf) {};

            void PrintStats(Scheduler::Stats& stats, std::ofstream& outputfile)
            {
              // Set formatting for floating-point values
              outputfile << std::fixed << std::setprecision(2);

              // Print server utilization
              outputfile << "(i) Server Utilization: " << stats.server_utilisation
                 << " (averaged over the simulation duration)\n";

              // Print Jain’s fairness index (JFI) and Related Fairness Bound
              // (RFB)
              outputfile << "(ii) Fairness Index (JFI) and Related Fairness Bound "
                    "(RFB):\n";
              outputfile << "    Time Unit   JFI      RFB\n";
              int time_unit = 0;
              while (!stats.jfi.empty() && !stats.rfb.empty()) {
                outputfile << "    " << time_unit << "        " << stats.jfi.front()
                   << "    " << stats.rfb.front() << "\n";
                stats.jfi.pop();
                stats.rfb.pop();
                time_unit += 10000; // Assuming RFB and JFI are computed every
                                    // 10000 time units
              }

              // Print average packet delay
              outputfile << "(iii) Average Packet Delay (across all packets): "
                 << stats.average_pkt_delay << "\n";

              // Print packet drop probability
              outputfile << "(iv) Packet Drop Probability (across all connections): "
                 << stats.packet_drop_prob << "\n";

              // Print per-source metrics as a table
              outputfile << "2. Per-Source Metrics:\n";
              outputfile << "Conn. ID | Total Packet Length Generated (BG) | Total "
                    "Packet Length Transmitted (BT) | BG/BT | Fraction of link "
                    "bandwidth allotted | Avg. Pkt Delay | Packet drop prob.\n";

              int conn_id = 1;
              for (const auto &src : stats.srcs) {
                outputfile << conn_id << "        | "
                   << src.total_packet_length_generated
                   << "                           | "
                   << src.total_packet_length_transmitted
                   << "                          | "
                   << src.percentage_transmitted << "  | "
                   << src.fraction_allocated << "                          | "
                   << src.averag_pkt_delay << "            | "
                   << src.packet_drop_prob << "\n";
                conn_id++;
              }
            }

            void Start(void) 
            {
                // initial setup
                Scheduler* schptr =new Scheduler(conf.n, conf.weights, TICKSIZE, conf.c, conf.b, conf.t);
                PacketGen::PacketSource::schedmutex = Scheduler::schedmutex;
                PacketGen::PacketSource::sched = schptr;
                schptr->StartProcessing();
                assert(conf.n == conf.pktSrcConfigs.size());
                std::cout << "Starting Packet Sources\n";

                for(int i=0; i<conf.n; i++)
                {
                    std::cout << "Starting Packet Source " << i << "\n";
                    auto pktsrc = new PacketGen::PacketSource(conf.pktSrcConfigs[i]);
                }

                // for(auto& pktsrc : pktsrcs)
                //     pktsrc.join();

                std::cout << conf.t << " is simulation time\n";
                std::this_thread::sleep_for(std::chrono::milliseconds((SETUP_TIME * 1000 + conf.t * TICKSIZE + BUFFER_TIME * 1000)));   // remove this voodoo constant

                Scheduler::Stats stats = schptr->GetStats();
                
                std::ofstream outputfile(conf.outputfilename);
                if(!outputfile.is_open())
                {
                    std::cerr << "Could not open outputfile\n";
                    exit(0);
                }

                PrintStats(stats, outputfile);

            }; // start the simulation for max time and write the values to the files

        private:

            Config conf;
    };
}