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
            link addLink(node src, int dst, float delay, float capacity); 
        
            void computeAPSP(void);

            // returns the connection ID (negative if connection failed)
            // required bandwidth is a tuple of (min, avg, max)
            conn addConnection(node src, int dst, std::tuple<int,int,int> reqd_bw);

            // returns the routing information
            RoutingTable getRoutingTable(node nd);

            // returns the forwarding table for the node
            ForwardingTable getForwardingTable(node nd);

            // returns the connections
            std::vector<std::shared_ptr<Connection>> getConnections(void);

            std::pair<int, int> getConnectionStats(void);

        private:

            class Link
            {
                public:
                    Link(node src, node dst, float delay, float capacity)
                        : src(src), dst(dst), delay(delay), capacity(capacity), available(capacity), numAllocs(0)
                        {};

                    bool allocate(float reqd_capacity);
                    bool deallocate(float removed_capacity);

                private:
                    node src, dst;
                    float delay, capacity;
                    float available;
                    int numAllocs;
            };

            class Node
            {
                public:

                    Node(node id) : id(id), ft(id) {};

                    void addNeighbour(node nid, std::shared_ptr<Link> link);

                    vcid generateVCID(void);

                    void addForwardingTableEntry(node in, vcid vin, node out, vcid vout);

                    std::shared_ptr<Link> getNeighbouringLink(node nbr);

                    ForwardingTable getForwardingTable(void) { return ft; }
                
                private:

                    node id;
                    std::unordered_map<node, std::shared_ptr<Link>> neighbours;
                    ForwardingTable ft;
                    std::set<vcid> vcids;
            };

            bool pessimistic;
            int connectionsTried;
            std::vector<Node> nodes;
            std::vector<std::shared_ptr<Link>> links;
            std::vector<std::shared_ptr<Connection>> connections;
            std::vector<RoutingTable> rtables;
    };
}