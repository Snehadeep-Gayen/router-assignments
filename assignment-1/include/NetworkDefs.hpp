#pragma once

#include <vector>
#include <iostream>
#include <unordered_map>

namespace Network{

    using node = int;
    using vcid = int;
    using link = int;
    using conn = int;

#define ROUTINGTABLE_LOGMODULE          "RoutingTable"
#define FORWARDTABLE_LOGMODULE          "ForwardTable"
    
    class RoutingTable
    {
        public:

            RoutingTable(node id) : id(id) {};

            struct Entry{
                node dst;
                std::vector<node> path;
                float delay;
                float cost;
            };

            void addEntry(node dst, Entry e);

            std::vector<node> getPath(node dst);

            float getCost(node dst);
            float getDelay(node dst);

            friend std::ostream& operator<<(std::ostream &os, const RoutingTable& rt);

        private:

            node id;
            std::unordered_map<node, Entry> entry;
    };

    class ForwardingTable
    {
        public:

            ForwardingTable(node id) : id(id) {};

            void addEntry(node in, vcid vin, node out, vcid vout);

            friend std::ostream& operator<<(std::ostream& os, const ForwardingTable& ft);
        
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

        friend std::ostream& operator<<(std::ostream& os, const struct Connection& con);
    };
}