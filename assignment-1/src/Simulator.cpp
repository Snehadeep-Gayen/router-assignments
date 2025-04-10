#include "Simulator.hpp"

namespace Simulation {

    using namespace Logging;

    int Simulator::Start(void) {
        LOGI(SIMULATOR_LOGMODULE, "Start function entered.");

        std::fstream topofile(conf.topofilename, std::ios::in);
        if (!topofile.is_open()) {
            LOGI(SIMULATOR_LOGMODULE, "Failed to open topology file.");
            return -1;
        }
        if (initialiseTopo(topofile) != 0) {
            LOGI(SIMULATOR_LOGMODULE, "Failed to initialize topology.");
            return -1;
        }
        topofile.close();

        if (compute() != 0) {
            LOGI(SIMULATOR_LOGMODULE, "Computation failed.");
            return -1;
        }

        std::fstream connfile(conf.connfilename, std::ios::in);
        if (!connfile.is_open()) {
            LOGI(SIMULATOR_LOGMODULE, "Failed to open connections file.");
            return -1;
        }
        if (addConnections(connfile) != 0) {
            LOGI(SIMULATOR_LOGMODULE, "Failed to add connections.");
            return -1;
        }
        connfile.close();

        std::fstream rtabfile(conf.rtabfilename, std::ios::out);
        std::fstream fwdfilename(conf.fwdfilename, std::ios::out);
        std::fstream pathsfile(conf.pathsfilename, std::ios::out);
        if (writeOutputs(rtabfile, fwdfilename, pathsfile) != 0) {
            LOGI(SIMULATOR_LOGMODULE, "Failed to write outputs.");
            return -1;
        }

        LOGI(SIMULATOR_LOGMODULE, "Start function exited successfully.");
        return 0;
    }

    int Simulator::initialiseTopo(std::fstream& topofile) 
    {
        LOGI(SIMULATOR_LOGMODULE, "initialiseTopo function entered.");

        // check from file 
        int edgecnt;
        topofile >> (this->noNodes) >> edgecnt;

        for(int i=0; i<noNodes; i++)
            control.addNode();

        int src, dst;
        float delay, capacity;

        while(edgecnt--)
        {
            topofile >> src >> dst >> delay >> capacity;
            // add the link
            control.addLink(src, dst, delay, capacity, conf.hop ? 1 : delay);
            // links are bidirectional, so add the reverse link also
            control.addLink(dst, src, delay, capacity, conf.hop ? 1 : delay);
        }

        LOGI(SIMULATOR_LOGMODULE, "initialiseTopo function exited.");
        return 0;
    }

    int Simulator::compute(void) {
        LOGI(SIMULATOR_LOGMODULE, "compute function entered.");
        control.computeKAPSP();
        LOGI(SIMULATOR_LOGMODULE, "compute function exited.");
        return 0;
    }

    int Simulator::addConnections(std::fstream& connfile) {
        LOGI(SIMULATOR_LOGMODULE, "addConnections function entered.");\
        int noConnections;
        connfile >> noConnections;
        int src, dst;
        float min_bw, avg_bw, max_bw;
        while(noConnections--)
        {
            connfile >> src >> dst >> min_bw >> avg_bw >> max_bw;
            control.addConnection(src, dst, std::tuple<float, float, float>{min_bw, avg_bw, max_bw});
        }
        LOGI(SIMULATOR_LOGMODULE, "addConnections function exited.");
        return 0;
    }

    void printSeparator(std::ostream& os)
    {
        for(int i=0; i<60; i++)
            os << "=";
        os << "\n";
    }

    int Simulator::writeOutputs(std::fstream& rtabfile, std::fstream& fwdfile, std::fstream& pathsfile) 
    {
        LOGI(SIMULATOR_LOGMODULE, "writeOutputs function entered.");

        // print the routing tables
        for(int i=0; i<noNodes; i++)
        {
            printSeparator(rtabfile);
            rtabfile << control.getRoutingTable(i);
            printSeparator(rtabfile);
            rtabfile << "Second Path ";
            rtabfile << control.getSecondRoutingTable(i);
            printSeparator(rtabfile);
        }

        // print the forwarding tables
        for(int i=0; i<noNodes; i++)
            fwdfile << control.getForwardingTable(i);
    
        // print the connections 
        const auto& stats = control.getConnectionStats();
        pathsfile << stats.first << " " << stats.second << "\n";
        const auto& connections = control.getConnections();
        for(const auto& connection : connections)
            pathsfile << *connection;

        LOGI(SIMULATOR_LOGMODULE, "writeOutputs function exited.");
        return 0;
    }
}
