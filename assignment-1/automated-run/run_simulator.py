# runs the binary on the input file and extracts output to results.csv

import os


BINARY_FOLDER = "./binaries"
TOPO_FOLDER = "./topofiles"
CONNECTION_FOLDER = "./connection-files/bursty"

# ./NetworkSimulator -top topology.txt -conn connections.txt -rt c -ft d -path e -flag hop -p 1

params = ['-top', 'topology.txt', '-conn', 'connections.txt', '-rt', 'c', '-ft', 'd', '-path', 'e', '-flag', 'dist', '-p', '1']

def main():

    os.system(f'cp {TOPO_FOLDER}/ARPANET-Topo.txt {BINARY_FOLDER}/topology.txt')

    # get all the files from the connection folder
    connection_files = os.listdir(CONNECTION_FOLDER)
    
    # run the binary on each file
    for connection_file in connection_files:

        if 'ARPA' in connection_file:
            continue

        # copy the connection file to the binary folder
        os.system(f"cp {CONNECTION_FOLDER}/{connection_file} {BINARY_FOLDER}/connections.txt")

        os.chdir(BINARY_FOLDER)

        NUM_TRIES = 10
        sum_num2 = 0
        for i in range(NUM_TRIES):
            os.system(f"./NetworkSimulator {' '.join(params)}")
            with open('e', 'r') as e:
                # read first two numbers and write to results.csv
                line = e.readline()
                num1, num2 = line.split()
                num1, num2 = int(num1), int(num2)
                sum_num2 += num2
        avg_num2 = sum_num2 / NUM_TRIES

        with open('results.csv', 'a') as f:
            f.write(f'{connection_file}, dist, pessimistic, {avg_num2/num1:.2f}\n')

        os.system('rm c d e')
        os.system('rm connections.txt')
        os.chdir('..')

    os.system(f'rm {BINARY_FOLDER}/topology.txt')

if __name__ == "__main__":
    main()


