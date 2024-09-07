#include "Benes.hpp"
#include "Logging.hpp"
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <iterator>

namespace Switch {


    std::vector<bool> Benes::SwitchPackets(const std::vector<int>& inputPackets)
    {
        std::vector<int> modPackets(numPorts, -1);
        std::copy(inputPackets.begin(), inputPackets.end(), modPackets.begin());

        // make it a permutation
        std::vector<bool> present(numPorts, false);
        std::for_each(inputPackets.begin(), inputPackets.end(), [&present](const int& val){ present[val] = true; });
        int modPacketIndex = inputPackets.size();
        for(int i=0; i<numPorts; i++)
            if(!present[i])
                modPackets[modPacketIndex++] = i;
        
        // now feed it to DeterminConfiguration
        DetermineConfiguration(modPackets);

        return {};
    }

    int Benes::GetShufflePosition(int pos)
    {
        return (2 * pos) % numPorts + (pos >= numPorts/2);
    }

    int Benes::GetInverseSufflePosition(int pos)
    {
        int candidate1 = pos/2 + numPorts/2;
        int candidate2 = pos/2;
        if(GetShufflePosition(candidate1)==pos)
            return candidate1;
        if(GetShufflePosition(candidate2)==pos)
            return candidate2;
        throw std::runtime_error("Cannot find the Pre-shuffle position for " + STR(pos) + " with " + STR(numPorts) + " ports.");
    }


    void Benes::TwoColorDFS(int node, std::vector<int>& color, const std::vector<std::vector<int>>& adj)
    {
        assert(color[node]==1 || color[node]==0);
        for(auto nbr : adj[node])
            if(color[nbr]!=0 && color[nbr]!=1)
            {
                color[nbr] = 1-color[node];
                TwoColorDFS(nbr, color, adj);
            }
            else if(color[nbr]==color[node])
                throw std::runtime_error("Graph is not 2 colorable!!!");
    }

    void Benes::DetermineConfiguration(const std::vector<int>& outputPorts)
    {
        std::string s;
        std::for_each(outputPorts.cbegin(), outputPorts.cend(), [&s](const int& val) { s += STR(val) + " "; });
        Logging::LOGI(BENES_LOGGING, "Got output ports array " + s);

        // no port should be negative
        assert(std::all_of(outputPorts.begin(), outputPorts.end(), [](const int& val) { return val>=0; }));

        // the number of ports should be same as the number of input packets
        assert(outputPorts.size()==numPorts);

        // the input must be a permutation
        std::vector<int> perm(outputPorts);
        std::sort(perm.begin(), perm.end());
        int pos = 0;
        std::for_each(perm.begin(), perm.end(), [&pos](const int& val) { assert(val==pos++); });


        // BASE CASE: if numPorts == 2 then just check
        if(numPorts==2)
        {
            if(outputPorts[0]==0)
            {
                this->configs = std::vector<std::vector<Switch::SwitchConfig>>(1, {SwitchConfig::THROUGH});
                this->PrintConfig();
                std::cout << "\n--------------\n";
            }
            else
            {
                this->configs = std::vector<std::vector<Switch::SwitchConfig>>(1, {SwitchConfig::CROSS});
                this->PrintConfig();
                std::cout << "\n--------------\n";
            }
            return;
        }

        // now form a graph with the outputPorts
        // nodes are the output ports of the packets 
        // from here onwards, whenever I refer to the output port i,
        // I'm referring to the packet destined at the output port i
        // there is an edge between output port i and output port j if either
        // 1. Both are inputted to the same 2x2 switch (that is input ports have only first bit different)
        // 2. Both are outputted to the same 2x2 switch (that is output ports have the only first bit different)

        // mapping that stores the input port corresponding to the output ports
        std::vector<int> inputPorts(outputPorts.size(), -1);
        for(int i=0; i<numPorts; i++)
            inputPorts[outputPorts[i]] = i;

        std::vector<std::vector<int>> adj(numPorts);
        for(int i=0; i<numPorts; i+=2)
        {
            int p1 = i, p2 = i+1;

            // input constraint
            adj[p1].push_back(p2);
            adj[p2].push_back(p1);
            Logging::LOGI(BENES_LOGGING, "Edge between " + STR(p1) + " " + STR(p2));

            // output constraint
            adj[outputPorts[p1]].push_back(outputPorts[p2]);
            adj[outputPorts[p2]].push_back(outputPorts[p1]);
            Logging::LOGI(BENES_LOGGING, "Edge between " + STR(outputPorts[p1]) + " " + STR(outputPorts[p2]));
        }

        // time to 2-color the graph
        std::vector<int> color(numPorts, -1);
        for(int i=0; i<numPorts; i++)
        {
            if(color[i]==-1)
            {
                int zeroCnt=0, oneCnt=0;
                for(auto i : color)
                    if(i==0)
                        zeroCnt++;
                    else if(i==1)
                        oneCnt++;
                color[i] = (oneCnt < zeroCnt);
                Benes::TwoColorDFS(i, color, adj);
                int cnt = 0;
                for(auto i : color)
                    if(i!=-1)
                        cnt++;
                std::cout << i << ":" << cnt << "\n";
            }
        }


        std::string colorstr;
        std::for_each(color.begin(), color.end(), [&colorstr](int val) { colorstr += STR(val) + " "; });
        Logging::LOGI(BENES_LOGGING, "Colors wrt output Ports are: "+colorstr);

        int zeroCnt=0, oneCnt=0;
        for(auto i : color)
            if(i==0)
                zeroCnt++;
            else if(i==1)
                oneCnt++;
        assert(zeroCnt == oneCnt);
        assert(std::all_of(color.begin(), color.end(), [](const int val){ return val==0 || val==1;}));

        // LOG the results
        for(int i=0; i<numPorts; i++)
        {
            Logging::LOGI(BENES_LOGGING, "Packet at input port " + STR(i) + " destined to port " +
                STR(outputPorts[i]) + " is being sent to the " + ((color[outputPorts[i]]==1)? "Lower Benes Half." : "Upper Benes Half."));
        }

        std::vector<Switch::SwitchConfig> firstSwitch, lastSwitch;
        for(int i=0; i<numPorts; i+=2)
        {
            assert(color[outputPorts[i]] != color[outputPorts[i+1]]);
            // if lower half is being sent to the lower half itself
            if(color[outputPorts[i]]==0)
                firstSwitch.push_back(Switch::SwitchConfig::THROUGH);
            else
                firstSwitch.push_back(Switch::SwitchConfig::CROSS);

            assert(color[i]!=color[i+1]);
            // if lower half is being sent to the lower half itself
            if(color[i]==0)
                lastSwitch.push_back(Switch::SwitchConfig::THROUGH);
            else
                lastSwitch.push_back(Switch::SwitchConfig::CROSS);
        }

        // Create a smaller Benes network
        const int halfSize = numPorts/2;
        Benes subBenes(halfSize);
        std::vector<int> topHalf(halfSize, -1), bottomHalf(halfSize, -1);
        for(int i=0; i<numPorts; i++)
        {
            int oldPort_i = inputPorts[i];
            int newPort_i = (inputPorts[i]/2)*2 + color[i];
            int shuffledPort_i = GetInverseSufflePosition(newPort_i);
            Logging::LOGI(BENES_LOGGING, "Input: OldPort_i: " + STR(oldPort_i) + ", NewPort_i: "+STR(newPort_i) + ", Shuffled Port: "+STR(shuffledPort_i));

            int oldPort_o = i;
            int newPort_o = (i/2)*2 + color[i];
            int shuffledPort_o = GetInverseSufflePosition(newPort_o);
            Logging::LOGI(BENES_LOGGING, "Output: OldPort_o: " + STR(oldPort_o) + ", NewPort_o: "+STR(newPort_o) + ", Shuffled Port: "+STR(shuffledPort_o));

            if(shuffledPort_i<halfSize)
            {
                topHalf[shuffledPort_i] = shuffledPort_o;
            }
            else
            {
                bottomHalf[shuffledPort_i-halfSize] = shuffledPort_o;
            }
        }
        
        // get the configuration for the top half
        std::vector<int> topHalfMod = topHalf;
        for(auto& outputPort : topHalfMod)
        {
            // std::cout << i << " hehe\n";
            // outputPort = GetInverseSufflePosition(outputPort) % halfSize;
            outputPort %= halfSize;
        }
        std::vector<int> bottomHalfMod = bottomHalf;
        for(auto& outputPort : bottomHalfMod)
        {
            // std::cout << i << " huhu\n";
            // outputPort = (GetInverseSufflePosition(outputPort)) % halfSize;
            outputPort %= halfSize;
        }

        subBenes.SwitchPackets(topHalfMod);
        auto topConfig = subBenes.GetConfigurations();
        Logging::LOGI(BENES_LOGGING, STR(topConfig.size()) + ": TopConfig size, n:"+ STR(numPorts) + ", 2log n - 1:" + STR(2*portLength - 1));
        assert(topConfig.size() == 2*portLength-3);
        subBenes.SwitchPackets(bottomHalfMod);
        auto bottomConfig = subBenes.GetConfigurations();
        assert(bottomConfig.size() == 2*portLength-3);

        std::vector<std::vector<Switch::SwitchConfig>> configs;

        // add the first layer of switch config
        configs.push_back(firstSwitch);
        std::cout << configs.size() << "hehe\n";
        // add all intermediate layers of switch config
        assert(topConfig.size()==bottomConfig.size());
        for(int i=0; i<topConfig.size(); i++)
        {
            std::vector<Switch::SwitchConfig> layerConfig;
            std::copy(topConfig[i].begin(), topConfig[i].end(), std::back_inserter(layerConfig));
            std::copy(bottomConfig[i].begin(), bottomConfig[i].end(), std::back_inserter(layerConfig));
            configs.push_back(layerConfig);
        }
        std::cout << configs.size() << "hehe\n";
        // add the end layer of switch config
        configs.push_back(lastSwitch);
        std::cout << configs.size() << "hehe\n";

        this->configs = configs;
        this->PrintConfig();
        std::cout << "\n-----------------------------------\n";
    }
}