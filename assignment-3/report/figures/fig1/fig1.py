import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a DataFrame
df = pd.read_csv('data.csv', index_col=0)

# Extracting 'p' values from the columns
p = df.columns.astype(float)

# Separate metrics into three lists: utilisation, drop_prob, and wait-time
metrics = ['Port Utilisation', 'Drop Probability', 'Waiting Time']
file_names = ['fig1a.png', 'fig1b.png', 'fig1c.png']

# Loop through each metric and plot the corresponding data
for i, metric in enumerate(metrics):
    plt.figure()  # Create a new figure for each plot
    for index, row in df.iterrows():
        if metric in index:  # Filter by the current metric
            plt.plot(p, row.values, marker='o', label=index.split('-')[0])
    
    # Add labels and title
    plt.xlabel('Packet Generation Probability (p)')
    plt.ylabel(metric.replace('_', ' ').title())
    plt.title(f'{metric.replace("_", " ").title()} vs p')
    plt.legend()
    plt.grid(True)
    
    # Save the plot
    plt.savefig(file_names[i])
    
    # Close the plot to avoid overlap in subsequent figures
    plt.close()

print("Plots saved as fig1a.png, fig1b.png, and fig1c.png")
