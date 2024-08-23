#!python3

# takes a topology file and generates a connection file
# generates all possible reachable pairs of nodes from the topology file
# randomly chooses a subset of these pairs to be connections

import sys
import random

TOPO_FOLDER = "./topofiles/"


# command line arguments
# 1. number of connections
# 2. topology file (if topofile name is a number, then nodes are randomly generated)
# 3. min, avg, max bandwidth


def main():

    # check for correct number of arguments
    if len(sys.argv) != 6:
        print(len(sys.argv))
        print("Usage: python3 connection-gen.py <num_connections> <topology_file> <min-bw> <avg-bw> <max-bw>")
        sys.exit(1)

    min_bw = float(sys.argv[3])
    avg_bw = float(sys.argv[4])
    max_bw = float(sys.argv[5])

    # get number of connections
    num_connections = int(sys.argv[1])

    # get topology file
    topofile = sys.argv[2]

    reachable_pairs = []

    # read topology file
    if topofile.isdigit():
        num_nodes = int(topofile)
        nodes = [i for i in range(num_nodes)]
        for i in nodes:
            for j in nodes:
                if i != j:
                    reachable_pairs.append((i, j))
    else:
        with open(TOPO_FOLDER + topofile, 'r') as f:
            lines = f.readlines()

            # get number of nodes
            num_nodes = int(lines[0].split()[0])

            # get nodes
            nodes = [i for i in range(num_nodes)]

            # get edges
            edges = []
            for line in lines[1:]:
                edge = line.split()
                edges.append((int(edge[0]), int(edge[1])))

            # do bfs to get all reachable pairs of nodes
            for i in nodes:
                visited = [False for i in range(num_nodes)]
                stack = []
                stack.append(i)
                visited[i] = True
                while stack:
                    node = stack.pop()
                    for edge in edges:
                        if edge[0] == node and not visited[edge[1]]:
                            stack.append(edge[1])
                            visited[edge[1]] = True
                            reachable_pairs.append((i, edge[1]))

    # choose random connections (may have duplicates)
    random_connections = random.choices(reachable_pairs, k=num_connections)

    # filename for connections file
    filename = "./connection-files/bursty/connections_" + str(num_connections) + "_" + topofile.split('.')[0] + "-" + str(min_bw)+ "-" + str(avg_bw) + "-" + str(max_bw) + ".txt"

    # write connections to file
    with open(filename, 'w') as f:
        f.write(str(num_connections) + '\n')
        for connection in random_connections:
            f.write(str(connection[0]) + ' ' + str(connection[1]) + ' ' + str(min_bw)+ " " + str(avg_bw) + " " + str(max_bw) + '\n')


if __name__ == "__main__":
    main()
