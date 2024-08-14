#pragma once

#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "NetworkDefs.hpp"

namespace Network {

    class Controller
    {
        public:

            // Use pessimistic to take the maximum bandwidth
            Controller(bool pessimistic=false);

            // returns the uid of the node
            node addNode(void); 

            // returns the uid of the unidirectional link (negative if failed)
            link addLink(int src, int dst, float delay, float capacity); 
        
            void computeAPSP(void);

            // returns the connection ID (negative if connection failed)
            // required bandwidth is a tuple of (min, avg, max)
            conn addConnection(int src, int dst, tuple<int,int,int> reqd_bw);

        private:

            class Link
            {
                public:
                    Link(node src, node dst, float delay, float capacity)
                        : src(src), dst(dst), delay(delay), capacity(capacity);

                    bool allocate(float reqd_capacity);

                private:
                    node src, dst;
                    float delay, capacity;
                    int numAllocs;
            };

            class Node
            {
                public:

                    Node(node id) : id(id), ft(id);

                    void addNeighbour(node nid, shared_ptr<Link> link);

                    vcid generateVCID(void);

                    void addForwardingTableEntry(node in, vcid vin, node out, vcid vout);

                    shared_ptr<Link> getNeighbouringLink(node nbr);
                
                private:

                    node id;
                    std::unordered_map<node, std::shared_ptr<Link>> neighbours;
                    ForwardingTable ft;
                    std::set<vcid> vcids;
            };

            std::vector<Node> nodes;
            std::vector<std::shared_ptr<Link>> links;
            std::vector<Connection> connections;
            std::vector<RoutingTable> rtables;
    };
}