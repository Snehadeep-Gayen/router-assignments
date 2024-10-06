import pandas as pd
from wrapper import run_simulation

# Parameters
N_values = [8, 16, 24, 32, 48, 64]
p_values = [0.4, 0.6, 0.8, 1.0]
K_values = [0.4, 0.7, 1.0]
L_values = [0.4, 0.7, 1.0]

# Prepare CSV files
utilization_csv_filename = './fig2/utilization_data.csv'
delay_csv_filename = './fig2/delay_data.csv'

# Initialize DataFrames for utilization and delay
utilization_data = []
delay_data = []

# Loop through all combinations of parameters
for N in N_values:
    for p in p_values:
        row_utilization = [p, N]
        row_delay = [p, N]

        # Collect data for each queue type
        # NOQ
        avg_delay, avg_utilization, _ = run_simulation(N, 100, p, 'NOQ')
        row_utilization.append(avg_utilization)
        row_delay.append(avg_delay)

        # INQ
        avg_delay, avg_utilization, _ = run_simulation(N, 100, p, 'INQ')
        row_utilization.append(avg_utilization)
        row_delay.append(avg_delay)

        # CIOQ for different K values
        for K in K_values:
            # CIOQ for different L values
            for L in L_values:
                K_value = K * N  # Calculate the K value based on N
                L_value = L * N  # Calculate the L value based on N
                avg_delay, avg_utilization, _ = run_simulation(N, 100, p, 'CIOQ', K_value, L_value)
                row_utilization.append(avg_utilization)
                row_delay.append(avg_delay)

        # Append rows to the respective data lists
        utilization_data.append(row_utilization)
        delay_data.append(row_delay)

# Convert lists to DataFrames
utilization_df = pd.DataFrame(utilization_data)
delay_df = pd.DataFrame(delay_data)

# Set column names
utilization_columns = ['p', 'N', 'NOQ', 'INQ'] + [f'CIOQ-{k}-{l}' for k in K_values for l in L_values]
delay_columns = ['p', 'N', 'NOQ', 'INQ'] + [f'CIOQ-{k}-{l}' for k in K_values for l in L_values]

utilization_df.columns = utilization_columns
delay_df.columns = delay_columns

# Save DataFrames to CSV files
utilization_df.to_csv(utilization_csv_filename, index=False)
delay_df.to_csv(delay_csv_filename, index=False)

print(f"Utilization data saved to {utilization_csv_filename}")
print(f"Delay data saved to {delay_csv_filename}")
