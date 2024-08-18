#pragma once

#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "NetworkDefs.hpp"

namespace Network {

#define CONTROLLER_LOGMODULE    "Controller"
#define NODE_LOGMODULE          "Node"
#define LINK_LOGMODULE          "Link"

    class Controller
    {
        public:

            // Use pessimistic to take the maximum bandwidth
            Controller(bool pessimistic=false) : pessimistic(pessimistic) {};

            // returns the uid of the node
            node addNode(void); 

            // returns the uid of the unidirectional link (negative if failed)
            link addLink(node src, int dst, float delay, float capacity, float cost); 
        
            void computeKAPSP(void);

            // returns the connection ID (negative if connection failed)
            // required bandwidth is a tuple of (min, avg, max)
            conn addConnection(node src, int dst, std::tuple<float,float,float> reqd_bw);

            // returns the routing information
            RoutingTable getRoutingTable(node nd);
            RoutingTable getSecondRoutingTable(node nd);

            // returns the forwarding table for the node
            ForwardingTable getForwardingTable(node nd);

            // returns the connections
            std::vector<std::shared_ptr<Connection>> getConnections(void);

            std::pair<int, int> getConnectionStats(void);

        private:

            class Link
            {
                public:
                    Link(node src, node dst, float delay, float capacity, float cost)
                        : src(src), dst(dst), delay(delay), capacity(capacity), available(capacity), numAllocs(0), cost(cost)
                        {};

                    bool allocate(float reqd_capacity);
                    bool deallocate(float removed_capacity);
                    float getDelay(void) { return delay; }
                    float getCost(void) { return cost; }
                    int getDst(void) { return dst; }

                private:
                    node src, dst;
                    float delay, capacity;
                    float available;
                    int numAllocs;
                    float cost;
            };

            class Node
            {
                public:

                    Node(node id) : id(id), ft(id) {};

                    void addNeighbour(node nid, std::shared_ptr<Link> link);

                    vcid generateVCID(void);

                    void addForwardingTableEntry(node in, vcid vin, node out, vcid vout);

                    std::shared_ptr<Link> getNeighbouringLink(node nbr);
                    std::vector<std::shared_ptr<Link>> getAllNeighbouringLinks(void);

                    ForwardingTable getForwardingTable(void) { return ft; }
                
                private:

                    node id;
                    std::unordered_map<node, std::shared_ptr<Link>> neighbours;
                    ForwardingTable ft;
                    std::set<vcid> vcids;
            };

            void computeKSSSP(int start_node);
            conn __addConnection(node src, node dst, float bw, std::vector<node>& path, float cost);
    

            bool pessimistic;
            bool hop;
            int connectionsTried = 0;
            std::vector<Node> nodes;
            std::vector<std::shared_ptr<Link>> links;
            std::vector<std::shared_ptr<Connection>> connections;
            std::vector<RoutingTable> rtables;
            std::vector<RoutingTable> rtables_snd; // second shortest path
    };
}