#include "NetworkDefs.hpp"
#include <iostream>


int main()
{
    Network::RoutingTable rt(1);
    Network::RoutingTable::Entry e = {2, {1, 2}, 0.1, 0.2};
    rt.addEntry(2, e);
    rt.addEntry(1, {1, {5, 2, 3, 4, 5, 6, 7, 8, 1}, 0.0, 0.0});
    std::cout << rt << std::endl;

    Network::ForwardingTable ft(1);
    std::cout << "Forwarding Table" << std::endl;
    ft.addEntry(1, 1, 2, 2);
    ft.addEntry(2, 2, 1, 1);
    std::cout << ft << std::endl;

    Network::Connection con = {1, 1, 2, {1, 2}, {1, 2}, 1.0};
    std::cout << "Connection" << std::endl;
    std::cout << con << std::endl;

    return 0;
}