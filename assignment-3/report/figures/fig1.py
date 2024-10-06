import csv
from wrapper import run_simulation

def create_csv_file(filename):
    packetgenprobs = [0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99]
    queue_types = ['NOQ', 'INQ', 'CIOQ']

    # Initialize data to write to CSV
    data = [['p'] + packetgenprobs]

    # Store the results for each queue type
    for queue in queue_types:
        avg_utilizations = []
        avg_drop_probs = []
        avg_waiting_times = []

        for p in packetgenprobs:
            if queue is not "CIOQ":
                avg_pd, avg_lu, avg_drop_prob = run_simulation(packetgenprob=p, queue_type=queue)
            else:
                avg_pd, avg_lu, avg_drop_prob = run_simulation(packetgenprob=p, queue_type=queue, knockout=4, inpq=6)

            if avg_pd is not None and avg_lu is not None and avg_drop_prob is not None:
                avg_utilizations.append(avg_lu)
                avg_drop_probs.append(avg_drop_prob)
                avg_waiting_times.append(avg_pd)  # Assuming Avg PD represents waiting time here
            else:
                avg_utilizations.append(None)
                avg_drop_probs.append(None)
                avg_waiting_times.append(None)

        # Append results to data list in the specified format
        data.append([f'{queue}-Port Utilisation'] + avg_utilizations)
        data.append([f'{queue}-Drop Probability'] + avg_drop_probs)
        data.append([f'{queue}-Waiting Time'] + avg_waiting_times)

    # Write data to CSV file
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(data)

    print(f"CSV file '{filename}' created successfully.")

if __name__ == "__main__":
    create_csv_file('fig1/data.csv')