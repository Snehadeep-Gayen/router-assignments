#include "NetworkDefs.hpp"
#include "Logging.hpp"
#include <sstream>
#include <cassert>

namespace Network {

    // RoutingTable class methods


    void RoutingTable::addEntry(node dst, Entry e) {
        assert(dst == e.dst);
        assert(dst == e.path.back());
        entry[dst]  = e;
        Logging::LOGI(ROUTINGTABLE_LOGMODULE, std::string("Added Routing table entry. Now size of routig table is ") + std::to_string(entry.size()));
    }

    std::vector<node> RoutingTable::getPath(node dst) {
        return entry.at(dst).path;
    }

    float RoutingTable::getCost(node dst) {
        return entry.at(dst).cost;
    }


    float RoutingTable::getDelay(node dst) {
        return entry.at(dst).delay;
    }

    std::ostream& operator<<(std::ostream &os, const RoutingTable& rt) 
    {
        os << "Routing Table for node " << rt.id << std::endl;
        for (const auto& [dst, e] : rt.entry) 
        {
            os << dst << ", ";
            for (auto n : e.path) {
                os << n;
                if(n == rt.entry.at(dst).path.back())
                    assert(n == dst);
                else
                    os << "->";
            }
            os << ", " << e.delay << ", " << e.cost << std::endl;
        }
        return os;
    }

    // ForwardingTable class methods

    void ForwardingTable::addEntry(node in, vcid vin, node out, vcid vout) {
        Entry e = {in, vin, out, vout};
        entries.push_back(e);
    }

    std::ostream& operator<<(std::ostream& os, const ForwardingTable& ft) 
    {
        for (const auto& e : ft.entries)
            os << ft.id << ", " << e.in << ", " << e.vin << ", " << e.out << ", " << e.vout << std::endl;
        return os;
    }

    // Connection struct methods

    std::ostream& operator<<(std::ostream& os, const Connection& con) 
    {
        if(con.path.empty())
            return os;
        os << con.id << ", " << con.src << ", " << con.dst << ", ";
        for (auto n : con.path) {
            os << n;
            if(n == con.dst)
                assert(n == con.path.back());
            else
                os << "->";
        }
        os << ", ";
        for (auto v : con.vcids) {
            os << v << " ";
        }
        os << ", " << con.pathcost << std::endl;
        return os;
    }
}
