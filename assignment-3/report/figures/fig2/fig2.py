import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV files into DataFrames
utilization_df = pd.read_csv('utilization_data.csv')
delay_df = pd.read_csv('delay_data.csv')

# Set font sizes for the plots
plt.rcParams.update({
    'font.size': 16,  # Default font size
    'axes.titlesize': 20,  # Title font size
    'axes.labelsize': 18,  # Axis label font size
    'xtick.labelsize': 16,  # X-axis tick label size
    'ytick.labelsize': 16,  # Y-axis tick label size
    'legend.fontsize': 16,  # Legend font size
})

# Extracting unique 'N' values from the DataFrames
unique_N = utilization_df['N'].unique()

# Loop through each unique value of N to create separate plots
for N in unique_N:
    # Filter data for the current value of N
    util_data = utilization_df[utilization_df['N'] == N]
    delay_data = delay_df[delay_df['N'] == N]

    # Plot Utilization
    plt.figure(figsize=(10, 7))   # Increased figure size
    plt.plot(util_data['p'], util_data['NOQ'], marker='o', label='NOQ', color='blue')
    plt.plot(util_data['p'], util_data['INQ'], marker='s', label='INQ', color='orange')
    plt.plot(util_data['p'], util_data[f'CIOQ-{0.4}-{0.4}'], marker='^', label='CIOQ-K=0.4,L=0.4', color='green')
    plt.plot(util_data['p'], util_data[f'CIOQ-{0.4}-{0.7}'], marker='v', label='CIOQ-K=0.4,L=0.7', color='red')
    plt.plot(util_data['p'], util_data[f'CIOQ-{0.4}-{1.0}'], marker='x', label='CIOQ-K=0.4,L=1.0', color='purple')

    # Set titles and labels
    # plt.title(f'Average Link Utilization vs Packet Generation Probability (p) for N={N}')
    plt.xlabel('Packet Generation Probability (p)')
    plt.ylabel('Link Utilization')
    plt.legend()
    plt.grid()
    plt.tight_layout()  # Adjust layout to prevent clipping
    plt.savefig(f'utilization_plot_N_{N}.png')
    plt.close()

    # Plot Delay
    plt.figure(figsize=(10, 7))  # Increased figure size
    plt.plot(delay_data['p'], delay_data['NOQ'], marker='o', label='NOQ', color='blue')
    plt.plot(delay_data['p'], delay_data['INQ'], marker='s', label='INQ', color='orange')
    plt.plot(delay_data['p'], delay_data[f'CIOQ-{0.4}-{0.4}'], marker='^', label='CIOQ-K=0.4,L=0.4', color='green')
    plt.plot(delay_data['p'], delay_data[f'CIOQ-{0.4}-{0.7}'], marker='v', label='CIOQ-K=0.4,L=0.7', color='red')
    plt.plot(delay_data['p'], delay_data[f'CIOQ-{0.4}-{1.0}'], marker='x', label='CIOQ-K=0.4,L=1.0', color='purple')

    # Set titles and labels
    # plt.title(f'Average Packet Delay vs Packet Generation Probability (p) for N={N}')
    plt.xlabel('Packet Generation Probability (p)')
    plt.ylabel('Packet Delay')
    plt.legend()
    plt.grid()
    plt.tight_layout()  # Adjust layout to prevent clipping
    plt.savefig(f'delay_plot_N_{N}.png')
    plt.close()

print("Utilization and delay plots saved as utilization_plot_N_<N>.png and delay_plot_N_<N>.png")
