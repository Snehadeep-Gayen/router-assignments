#include "Controller.hpp"
#include "Logging.hpp"
#include <sstream>

namespace Network {

    node Controller::addNode(void) 
    {
        nodes.emplace_back(Node(nodes.size()));

        Logging::LOGI(CONTROLLER_LOGMODULE, 
            std::string("added Node ") + std::to_string(nodes.size() - 1));

        return nodes.size() - 1;
    }

    link Controller::addLink(node src, int dst, float delay, float capacity)
    {
        assert(src < nodes.size());
        assert(dst < nodes.size());
        assert(delay >= 0);
        assert(capacity >= 0);

        links.emplace_back(std::make_shared<Link>(src, dst, delay, capacity));
        nodes[src].addNeighbour(dst, links.back());

        Logging::LOGI(CONTROLLER_LOGMODULE, 
            std::string("added Link ") + std::to_string(links.size() - 1));

        return links.size() - 1;
    }

    void Controller::computeAPSP(void) 
    {
        std::stringstream ss;
        ss << "computeAPSP function entered.";
        Logging::LOGI(CONTROLLER_LOGMODULE, ss.str());

        // initialize the routing tables

        Logging::LOGI(CONTROLLER_LOGMODULE, "computeAPSP function exited.");
    }

    conn Controller::addConnection(node src, int dst, std::tuple<int, int, int> reqd_bw) 
    {
        assert(src < nodes.size());
        assert(dst < nodes.size());

        // the id of the connection is the number of connections tried so far
        // but the index of the connection in the connections vector is number_of_successful_connections - 1
        connectionsTried++;

        float min_bw = std::get<0>(reqd_bw);
        float avg_bw = std::get<1>(reqd_bw);
        float max_bw = std::get<2>(reqd_bw);
        float bw = pessimistic ? max_bw : avg_bw + 0.35 * (max_bw - avg_bw);

        // get the path from the routing table
        std::vector<node> path = rtables[src].getPath(dst);
        std::vector<vcid> vcids;

        // every link in the path should have the required capacity
        for (size_t i = 0; i < path.size() - 1; i++) {

            std::shared_ptr<Link> link = nodes[path[i]].getNeighbouringLink(path[i+1]);

            // check if the link has the required capacity
            if(!link->allocate(bw)) {
                Logging::LOGE(CONTROLLER_LOGMODULE, "Connection failed due to insufficient bandwidth.");
                // deallocate the bandwidth allocated so far
                for (size_t j = 0; j < i; j++)
                    nodes[path[j]].getNeighbouringLink(path[j+1])->deallocate(bw);
                return -1;
            }
        }

        // generate VCIDs for the connection
        for (size_t i = 0; i < path.size() - 1; i++) {
            vcids.push_back(nodes[path[i]].generateVCID());
        }

        // except the source and destination, add the forwarding table entries
        for(size_t i = 1; i < path.size() - 2; i++) {
            nodes[path[i]].addForwardingTableEntry(path[i-1], vcids[i-1], path[i+1], vcids[i]);
        }

        // add the connection to the list of connections
        connections.emplace_back(std::make_shared<Connection>(connectionsTried, src, dst, path, vcids));

        return connections.size()-1;
    }

    RoutingTable Controller::getRoutingTable(node nd) 
    {
        return rtables[nd];
    }

    ForwardingTable Controller::getForwardingTable(node nd) 
    {
        return nodes[nd].getForwardingTable();
    }

    std::vector<std::shared_ptr<Connection>> Controller::getConnections(void) {
        return connections;
    }

    std::pair<int, int> Controller::getConnectionStats(void)
    {
        return std::make_pair(connectionsTried, connections.size());
    }

    // Link class methods
    bool Controller::Link::allocate(float reqd_capacity) 
    {
        if(reqd_capacity > available) {
            return false;
        }
        available -= reqd_capacity;
        numAllocs++;
        return true;
    }

    bool Controller::Link::deallocate(float removed_capacity) 
    {
        available += removed_capacity;
        numAllocs--;
        assert(available <= capacity);
        return true;
    }

    // Node class methods
    void Controller::Node::addNeighbour(node nid, std::shared_ptr<Link> link) 
    {
        neighbours[nid] = link;
    }

    vcid Controller::Node::generateVCID(void) 
    {
        // generate a random number and check if it is already in use
        vcid v = rand();
        while(vcids.find(v) != vcids.end())
            v = rand();
        vcids.insert(v);
        return v;
    }

    void Controller::Node::addForwardingTableEntry(node in, vcid vin, node out, vcid vout) 
    {
        ft.addEntry(in, vin, out, vout);
    }

    std::shared_ptr<Controller::Link> Controller::Node::getNeighbouringLink(node nbr) 
    {
        assert(neighbours.find(nbr) != neighbours.end());
        return neighbours.at(nbr);
    }
}
