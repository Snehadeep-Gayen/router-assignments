#include "simulator.h"
#include <arpa/inet.h>  // For inet_pton
#include "htrie.h"

using namespace Simulation;

NetIP::NetIP(uint32_t ip, int maskbits) : ip(ip), bits(maskbits)
{
    std::bitset<32> maskset;
    for(int i=0; i<maskbits; i++)
        maskset[31-i] = 1;  // 0 is the least significant digit
    mask = maskset.to_ulong();
}

NetIP::NetIP(std::string netmask_string)
{
    auto pos = netmask_string.find("/");
    if(pos==std::string::npos)
    {
        LOGE(SIMULATOR_LOGMODULE, "Rule parse error" + netmask_string);
        throw std::runtime_error("Rule parse error");
    }
    std::string ipstr = netmask_string.substr(0, pos);
    int maskbits = std::stoi(netmask_string.substr(pos+1, netmask_string.size()-(pos+1)));
    uint32_t ip;
    if (inet_pton(AF_INET, ipstr.c_str(), &ip) != 1) {
        throw std::runtime_error("Invalid IPv4 address format");
    }

    NetIP::ip = ip;
    NetIP::bits = maskbits;
    std::bitset<32> maskset;
    for(int i=0; i<maskbits; i++)
        maskset[31-i] = 1;  // 0 is the least significant digit
    mask = maskset.to_ulong();

}

bool NetIP::match(uint32_t ipaddr)
{
    if((ip&mask) == (ipaddr&mask))
        return true;
    return false;
}

void Simulator::Start()
{
    // start by setting up the trie
    HTrie<HTrie<std::vector<int>>> ht;
    int ruleno = 0;
    for(const auto& rule : rules)
    {
        auto ht1 = ht.insert(NetIP::MakeVector(rule.i1));
        auto ruleno_ptr = ht1->insert(NetIP::MakeVector(rule.i2));
        ruleno_ptr->push_back(ruleno);
        ruleno++;
    }

    // next search for all the matches
    for(const auto& ip : tests)
    {
        std::vector<int> matchedRules;
        auto vecHTries = ht.search(HTrie<int>::IntToPath(ip.first));    // this is bad, change this later
        for(auto itr=vecHTries.rbegin(); itr!=vecHTries.rend(); itr++)
        {
            auto vecVecInts = (*itr)->search(HTrie<int>::IntToPath(ip.second));
            if(!vecVecInts.empty())
                matchedRules = *vecVecInts.back();   // last of the matched ints
        }
        std::cout << "Search matched: ";
        for(auto i : matchedRules)
            std::cout << i << " ";
        std::cout << std::endl;
    }
}


Simulator::Simulator(struct Config&& conf) : conf(conf), rules(), tests()
{  
    // get all the rules from the file
    std::ifstream rulesfile(conf.rulesfilename);
    if(!rulesfile.is_open())
    {
        LOGE(SIMULATOR_LOGMODULE, "Invalid Rulesfile" + conf.rulesfilename);
        exit(0);
    }

    // import all the rules
    while(!rulesfile.eof())
    {
        int ruleno;
        std::string netmask1, netmask2;
        rulesfile >> netmask1 >> netmask2;
        if(netmask1.empty() or netmask2.empty())
            continue;
        rules.push_back({.i1 = {netmask1}, .i2 = {netmask2}});
    }

    std::ifstream inputfile(conf.inputfilename);
    if(!inputfile.is_open())
    {
        LOGE(SIMULATOR_LOGMODULE, "Invalid Inputfile" + conf.inputfilename);
        exit(0);
    }

    // import all the testcases
    while(!inputfile.eof())
    {
        std::string ip1, ip2;
        inputfile >> ip1 >> ip2;
        if(ip1.empty() or ip2.empty())
            continue;
        uint32_t ipnum1, ipnum2;
        if (inet_pton(AF_INET, ip1.c_str(), &ipnum1) != 1)
            throw std::runtime_error("Invalid IPv4 address format");
        if (inet_pton(AF_INET, ip2.c_str(), &ipnum2) != 1)
            throw std::runtime_error("Invalid IPv4 address format");
        Simulator::tests.push_back({ipnum1, ipnum2});
    }

}

std::vector<bool> NetIP::MakeVector(NetIP nip)
{
    std::vector<bool> path;
    std::bitset<32> ip(nip.ip);
    for(int i=0; i<nip.bits; i++)
        path.push_back(ip[31-i]);
    return path;
}