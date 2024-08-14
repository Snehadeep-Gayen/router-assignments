#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

namespace Network{

    using node = int;
    using link = int;
    using conn = int;

    
    class RoutingTable
    {
        public:

            RoutingTable(node id);

            struct Entry{
                node dst;
                std::vector<node> path;
                float delay;
                float cost;
            };

            void addEntry(node dst, Entry e);

            std::vector<node> getPath(node dst);

            float getCost(node dst);

            friend ostream& operator<<(ostream &os, const RoutingTable& rt);

        private:

            node id;
            std::unordered_map<node, Entry> entry;
    };

    class ForwardingTable
    {
        public:

            Forwarding(node id);

            void addEntry(node in, vcid vin, node out, vcid vout);

            friend ostream& operator<<(ostream& os, const ForwardingTable& ft);
        
        private:

            struct Entry{
                node in;
                vcid vin;
                node out;
                vcid vout;
            };

            node id;
            std::vector<Entry> entries; // ideally this should be std::unordered_map from std::pair<node, vcid> to Entry
    };

    struct Connection
    {
        conn id;
        node src;
        node dst;
        std::vector<node> path;
        std::vector<vcid> vcids;
        float pathcost;

        friend ostream& operator<<(ostream& os, const struct Connection& con);
    };
}