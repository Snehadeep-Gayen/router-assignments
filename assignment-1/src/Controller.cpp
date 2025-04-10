#include "Controller.hpp"
#include "Logging.hpp"
#include <sstream>
#include <cassert>
#include <vector>
#include <queue>

namespace Network {

    node Controller::addNode(void) 
    {
        rtables.push_back(nodes.size());
        rtables_snd.push_back(nodes.size());
        nodes.emplace_back(Node(nodes.size()));

        Logging::LOGI(CONTROLLER_LOGMODULE, 
            std::string("added Node ") + std::to_string(nodes.size() - 1));

        return nodes.size() - 1;
    }

    link Controller::addLink(node src, node dst, float delay, float capacity, float cost)
    {
        Logging::LOGI(CONTROLLER_LOGMODULE,
            std::string("Adding link from " + std::to_string(src) + " to " + std::to_string(dst)));
        assert(src < (int) nodes.size());
        assert(dst < (int) nodes.size());
        assert(delay >= 0);
        assert(capacity >= 0);

        links.emplace_back(std::make_shared<Link>(src, dst, delay, capacity, cost));
        nodes[src].addNeighbour(dst, links.back());

        Logging::LOGI(CONTROLLER_LOGMODULE, 
            std::string("added Link ") + std::to_string(links.size() - 1));

        return links.size() - 1;
    }


    void Controller::computeKSSSP(int start_node)
    {
        Logging::LOGI(CONTROLLER_LOGMODULE, std::string("Computing KSSSP for node ") + std::to_string(start_node));

        // initialize the routing tables
        struct Qelem
        {
            std::vector<int> path;
            float cost;
            float delay;
        };

        struct QComp
        {
            bool operator() (const Qelem& lhs, const Qelem& rhs)
            {
                return lhs.cost > rhs.cost;
            };
        };

        std::priority_queue<Qelem, std::vector<Qelem>, QComp> pq;

        RoutingTable firstpath(start_node);
        RoutingTable secondpath(start_node);
        std::vector<bool> isFirstPath(nodes.size(), false);
        std::vector<bool> isSecondPath(nodes.size(), false);

        pq.push({
            .path={start_node}, 
            .cost=0, 
            .delay=0});

        int iter = 0;
        while(!pq.empty())
        {
            Logging::LOGI(CONTROLLER_LOGMODULE, std::string("Iteration: ")+std::to_string(iter++));
            auto qelem = pq.top();
            int node = qelem.path.back();

            pq.pop();

            if(start_node!=node && isFirstPath[node] && isSecondPath[node])
                continue;
            else if(start_node!=node)
            {
                if(!isFirstPath[node])
                {
                    firstpath.addEntry(node, RoutingTable::Entry{ 
                        .dst = node,
                        .path = qelem.path,
                        .delay = qelem.delay,
                        .cost = qelem.cost});

                    // Logging for firstpath addition
                    std::stringstream ss;
                    ss << "First path added for node " << node << ". Path: [";
                    for (const auto& n : qelem.path) {
                        ss << n << " ";
                    }
                    ss << "], delay: " << qelem.delay << ", cost: " << qelem.cost;
                    Logging::LOGI(CONTROLLER_LOGMODULE, ss.str());
                    
                    isFirstPath[node] = true;
                }
                else
                {
                    assert(!isSecondPath[node]);

                    secondpath.addEntry(node, RoutingTable::Entry{ 
                        .dst = node,
                        .path = qelem.path,
                        .delay = qelem.delay,
                        .cost = qelem.cost});


                    // Logging for secondpath addition
                    std::stringstream ss;
                    ss << "Second path added for node " << node << ". Path: [";
                    for (const auto& n : qelem.path) {
                        ss << n << " ";
                    }
                    ss << "], delay: " << qelem.delay << ", cost: " << qelem.cost;
                    Logging::LOGI(CONTROLLER_LOGMODULE, ss.str());


                    isSecondPath[node] = true;
                }
            }

            // const Node& nodeObj = nodes[node];
            for(const auto& linkptr : nodes[node].getAllNeighbouringLinks())
            {
                float linkcost = linkptr->getCost();
                int dst = linkptr->getDst();
                float linkdelay = linkptr->getDelay();

                if(!isFirstPath[dst] || !isSecondPath[dst])
                {
                    Qelem temp;
                    temp.cost = {qelem.cost+linkcost};
                    temp.delay = {qelem.delay+linkdelay};
                    temp.path = qelem.path;
                    temp.path.push_back(dst);

                    pq.push(temp);
                }
            }
        }
        
        rtables[start_node] = (firstpath);
        rtables_snd[start_node] = (secondpath);
    }
    
    void Controller::computeKAPSP(void) // K All-pair shortest path
    {
        std::stringstream ss;
        ss << "computeAPSP function entered.";
        Logging::LOGI(CONTROLLER_LOGMODULE, ss.str());

        for(size_t i=0; i<nodes.size(); i++)
            computeKSSSP(i);

        Logging::LOGI(CONTROLLER_LOGMODULE, "computeAPSP function exited.");
    }

    conn Controller::__addConnection(node src, node dst, float bw, std::vector<node>& path, float cost)
    {
        std::vector<vcid> vcids;

        // every link in the path should have the required capacity
        for (size_t i = 0; i < path.size() - 1; i++) 
        {
            std::shared_ptr<Link> link = nodes[path[i]].getNeighbouringLink(path[i+1]);

            // check if the link has the required capacity
            if(!link->allocate(bw)) 
            {
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
        for(size_t i = 1; i <= path.size() - 2; i++) {
            nodes[path[i]].addForwardingTableEntry(path[i-1], vcids[i-1], path[i+1], vcids[i]);
        }

        // add the connection to the list of connections
        connections.push_back(std::make_shared<Connection>(Connection{
            .id = connectionsTried, 
            .src = src, 
            .dst = dst, 
            .path = path, 
            .vcids = vcids, 
            .pathcost = cost}));

        return connections.size()-1;
    }

    conn Controller::addConnection(node src, int dst, std::tuple<float, float, float> reqd_bw) 
    {
        assert(src < (int) nodes.size());
        assert(dst < (int) nodes.size());

        // the id of the connection is the number of connections tried so far
        // but the index of the connection in the connections vector is number_of_successful_connections - 1
        connectionsTried++;

        float min_bw = std::get<0>(reqd_bw);
        float avg_bw = std::get<1>(reqd_bw);
        float max_bw = std::get<2>(reqd_bw);
        float bw = pessimistic ? max_bw : std::min((double)max_bw, avg_bw + 0.35 * (max_bw - min_bw));
        Logging::LOGI(CONTROLLER_LOGMODULE, std::to_string(bw) + std::string(" bandwidth required"));

        std::vector<node> path;
        float cost;

        // get the path from the routing table
        try
        {
            path = rtables[src].getPath(dst);
            cost = rtables[src].getCost(dst);

            conn connid = __addConnection(src, dst, bw, path, cost);
            if(connid>=0)
                return connid;

        }
        catch(const std::out_of_range&)
        {
            Logging::LOGI(CONTROLLER_LOGMODULE, "Could not find path from src to destination");
        }

        Logging::LOGI(CONTROLLER_LOGMODULE, "First shortest path cannot be used");
        

        try
        {
            path = rtables_snd[src].getPath(dst);
            cost = rtables_snd[src].getCost(dst);

            return __addConnection(src, dst, bw, path, cost);
        }
        catch(const std::out_of_range&)
        {
            Logging::LOGI(CONTROLLER_LOGMODULE, "Could not find second shortest path from src to destination");
            return -1;   
        }
    }

    RoutingTable Controller::getRoutingTable(node nd) 
    {
        return rtables.at(nd);
    }

    RoutingTable Controller::getSecondRoutingTable(node nd) 
    {
        return rtables_snd.at(nd);
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
        std::stringstream str;
        str << "Asking for capacity " << reqd_capacity << " in link with capacity " << available;
        Logging::LOGI(CONTROLLER_LOGMODULE, str.str());
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
        vcid v = rand()%(1<<VCID_BITS);
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

    std::vector<std::shared_ptr<Controller::Link>> Controller::Node::getAllNeighbouringLinks()
    {
        std::vector<std::shared_ptr<Link>> links;
        for(auto& [_, link] : neighbours)
            links.push_back(link);
        return links;
    }
}
