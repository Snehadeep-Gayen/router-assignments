#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "htrie.h"  // Include your HTrie class definition
#include "simulator.h"


// Function to run the test cases
void runTests2() {
    // Prepare test data
    std::vector<Simulation::Rule> rules = {
        { Simulation::NetIP(192168001, 24), Simulation::NetIP(192168002, 24) }, // Example rule 1
        { Simulation::NetIP(10, 8), Simulation::NetIP(10, 8) },                // Example rule 2
        { Simulation::NetIP(172160000, 12), Simulation::NetIP(172161000, 12) } // Example rule 3
    };

    // Simulate test IPs
    std::vector<std::pair<int, int>> tests = {
        { 192168001, 192168002 }, // Should match rule 1
        { 10, 10 },               // Should match rule 2
        { 172160000, 172161000 }, // Should match rule 3
        { 255, 255 }              // Should not match any rules
    };

    // Start the HTrie setup
    HTrie<HTrie<std::vector<int>>> ht;
    int ruleno = 0;

    // Insert rules into the trie
    for (const auto& rule : rules) {
        auto ht1 = ht.insert(Simulation::NetIP::MakeVector(rule.i1));
        auto ruleno_ptr = ht1->insert(Simulation::NetIP::MakeVector(rule.i2));
        ruleno_ptr->push_back(ruleno);
        ruleno++;
    }

    // Search for matches and validate results
    for (const auto& ip : tests) {
        std::vector<int> matchedRules;
        auto vecHTries = ht.search(HTrie<int>::IntToPath(ip.first));
        
        for (auto itr = vecHTries.rbegin(); itr != vecHTries.rend(); itr++) {
            auto vecVecInts = (*itr)->search(HTrie<int>::IntToPath(ip.second));
            if (!vecVecInts.empty())
                matchedRules = *vecVecInts.back(); // last of the matched ints
        }

        // Validate output based on expected results
        if (ip.first == 192168001 && ip.second == 192168002) {
            assert(matchedRules.size() == 1 && matchedRules[0] == 0);
            std::cout << "Test passed for: " << ip.first << ", " << ip.second << std::endl;
        } else if (ip.first == 10 && ip.second == 10) {
            assert(matchedRules.size() == 1 && matchedRules[0] == 1);
            std::cout << "Test passed for: " << ip.first << ", " << ip.second << std::endl;
        } else if (ip.first == 172160000 && ip.second == 172161000) {
            assert(matchedRules.size() == 1 && matchedRules[0] == 2);
            std::cout << "Test passed for: " << ip.first << ", " << ip.second << std::endl;
        } else {
            assert(matchedRules.size() == 1 && matchedRules[0] == 1);
            std::cout << "Test passed for: " << ip.first << ", " << ip.second << std::endl;
        }
    }
}


void runTests()
{
    // Test case 1: Inserting and searching a single value
    HTrie<int> trie;

    // Insert 5 at path [1, 0, 1] (binary representation of 5)
    std::vector<bool> path1 = {1, 0, 1};
    *trie.insert(path1) = 5;

    // Search for 5
    auto found = trie.search(path1);
    assert(found.size() == 1 && *found[0] == 5);
    std::cout << "Test case 1 passed!" << std::endl;

    // Test case 2: Inserting and searching multiple values
    // Insert 10 at path [1, 0, 0] (binary representation of 10)
    std::vector<bool> path2 = {1, 0, 0};
    *trie.insert(path2) = 10;

    // Insert 3 at path [0, 0, 1] (binary representation of 3)
    std::vector<bool> path3 = {0, 0, 1};
    *trie.insert(path3) = 3;

    // Search for 10
    found = trie.search(path2);
    assert(found.size() == 1 && *found[0] == 10);
    std::cout << "Test case 2 passed!" << std::endl;

    // Search for 3
    found = trie.search(path3);
    assert(found.size() == 1 && *found[0] == 3);
    std::cout << "Test case 3 passed!" << std::endl;

    // Test case 4: Search for a path that does not exist
    std::vector<bool> path4 = {0, 1, 1}; // Not inserted
    found = trie.search(path4);
    assert(found.empty());
    std::cout << "Test case 4 passed!" << std::endl;

    // Test case 5: Search for a partial path
    // Insert value 7 at path [1, 1] (binary representation of 7)
    std::vector<bool> path5 = {1, 1};
    *trie.insert(path5) = (7);

    // Search for path [1, 1] - should find 7
    found = trie.search(path5);
    assert(found.size() == 1 && *found[0] == 7);
    std::cout << "Test case 5 passed!" << std::endl;

    runTests2();
}

int main()
{
    runTests();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
