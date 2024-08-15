#include "NetworkDefs.hpp"
#include "Logging.hpp"
#include <sstream>

namespace Network {

    // RoutingTable class methods

    RoutingTable::RoutingTable(node id) : id(id) {
        std::stringstream ss;
        ss << "RoutingTable constructor entered. id: " << id;
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, ss.str());
        // Initialization logic here
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, "RoutingTable constructor exited.");
    }

    void RoutingTable::addEntry(node dst, Entry e) {
        std::stringstream ss;
        ss << "addEntry function entered. dst: " << dst << ", path: [";
        for (const auto& n : e.path) {
            ss << n << " ";
        }
        ss << "], delay: " << e.delay << ", cost: " << e.cost;
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, ss.str());

        // Logic to add an entry
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, "addEntry function exited.");
    }

    std::vector<node> RoutingTable::getPath(node dst) {
        std::stringstream ss;
        ss << "getPath function entered. dst: " << dst;
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, ss.str());
        // Logic to get the path
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, "getPath function exited.");
        return {}; // Placeholder return value
    }

    float RoutingTable::getCost(node dst) {
        std::stringstream ss;
        ss << "getCost function entered. dst: " << dst;
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, ss.str());
        // Logic to get the cost
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, "getCost function exited.");
        return 0.0f; // Placeholder return value
    }

    std::ostream& operator<<(std::ostream &os, const RoutingTable& rt) {
        std::stringstream ss;
        ss << "operator<< for RoutingTable entered.";
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, ss.str());
        // Logic to output the routing table
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, "operator<< for RoutingTable exited.");
        return os;
    }

    // ForwardingTable class methods

    ForwardingTable::ForwardingTable(node id) : id(id) {
        std::stringstream ss;
        ss << "ForwardingTable constructor entered. id: " << id;
        Logging::LOGI(FORWARDTABLE_LOGMODULE, ss.str());
        // Initialization logic here
        Logging::LOGI(FORWARDTABLE_LOGMODULE, "ForwardingTable constructor exited.");
    }

    void ForwardingTable::addEntry(node in, vcid vin, node out, vcid vout) {
        std::stringstream ss;
        ss << "addEntry function entered. in: " << in << ", vin: " << vin 
           << ", out: " << out << ", vout: " << vout;
        Logging::LOGI(FORWARDTABLE_LOGMODULE, ss.str());
        // Logic to add an entry
        Logging::LOGI(FORWARDTABLE_LOGMODULE, "addEntry function exited.");
    }

    std::ostream& operator<<(std::ostream& os, const ForwardingTable& ft) {
        std::stringstream ss;
        ss << "operator<< for ForwardingTable entered.";
        Logging::LOGI(FORWARDTABLE_LOGMODULE, ss.str());
        // Logic to output the forwarding table
        Logging::LOGI(FORWARDTABLE_LOGMODULE, "operator<< for ForwardingTable exited.");
        return os;
    }

    // Connection struct methods

    std::ostream& operator<<(std::ostream& os, const Connection& con) {
        std::stringstream ss;
        ss << "operator<< for Connection entered.";
        Logging::LOGI(FORWARDTABLE_LOGMODULE, ss.str());
        // Logic to output the connection details
        Logging::LOGI(FORWARDTABLE_LOGMODULE, "operator<< for Connection exited.");
        return os;
    }
}
