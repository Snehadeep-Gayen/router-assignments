import matplotlib.pyplot as plt
import pandas as pd

# Data for ARPANET and NFSNET
data = {
    'topology': ['ARPANET', 'ARPANET', 'ARPANET', 'ARPANET', 'ARPANET', 'ARPANET', 'NFSNET', 'NFSNET', 'NFSNET', 'NFSNET', 'NFSNET', 'NFSNET'],
    'type': ['opti', 'opti', 'opti', 'pessi', 'pessi', 'pessi', 'opti', 'opti', 'opti', 'pessi', 'pessi', 'pessi'],
    'num_connections': [100, 200, 300, 100, 200, 300, 100, 200, 300, 100, 200, 300],
    2: [1.00, 1.00, 1.00, 1.00, 1.00, 0.99, 1.00, 1.00, 1.00, 1.00, 0.81, 0.77],
    4: [1.00, 1.00, 1.00, 1.00, 0.96, 0.82, 1.00, 0.82, 0.68, 0.78, 0.60, 0.50],
    6: [1.00, 1.00, 0.95, 1.00, 0.86, 0.69, 0.98, 0.73, 0.68, 0.77, 0.55, 0.46],
    8: [1.00, 0.97, 0.88, 0.94, 0.78, 0.64, 0.87, 0.70, 0.61, 0.71, 0.45, 0.38],
    10: [1.00, 1.00, 0.83, 0.90, 0.79, 0.58, 0.74, 0.59, 0.52, 0.57, 0.36, 0.30],
    12: [1.00, 0.87, 0.75, 0.80, 0.67, 0.56, 0.70, 0.59, 0.43, 0.48, 0.39, 0.27],
    14: [0.96, 0.83, 0.66, 0.77, 0.59, 0.47, 0.63, 0.53, 0.40, 0.43, 0.35, 0.26],
    16: [0.96, 0.82, 0.70, 0.71, 0.57, 0.46, 0.75, 0.52, 0.37, 0.54, 0.33, 0.25],
    18: [0.92, 0.72, 0.62, 0.67, 0.47, 0.43, 0.72, 0.43, 0.37, 0.47, 0.28, 0.21],
    20: [0.95, 0.74, 0.62, 0.73, 0.54, 0.40, 0.66, 0.45, 0.37, 0.46, 0.28, 0.23]
}

# Convert data to pandas DataFrame
df = pd.DataFrame(data)

# Set up the figure and subplots
fig, axs = plt.subplots(1, 2, figsize=(14, 6))

# Plot Optimistic Results
for connections in df[(df['type'] == 'opti')]['num_connections'].unique():
    subset = df[(df['type'] == 'opti') & (df['num_connections'] == connections)]
    axs[0].plot(subset.columns[3:], 1-subset.iloc[0, 3:], marker='o', label=f"{subset['topology'].values[0]} - {connections} connections")

# Plot Pessimistic Results
for connections in df[(df['type'] == 'pessi')]['num_connections'].unique():
    subset = df[(df['type'] == 'pessi') & (df['num_connections'] == connections)]
    axs[1].plot(subset.columns[3:], 1-subset.iloc[0, 3:], marker='o', label=f"{subset['topology'].values[0]} - {connections} connections")

# Labels and Titles
axs[0].set_title("Optimistic Constraint", fontsize=20)
axs[0].set_xlabel("Bandwidth Values", fontsize=16)
axs[0].set_ylabel("Blocking Probability", fontsize=16)
axs[0].set_xticks(range(2, 21, 2), fontsize=16)
axs[0].legend(fontsize=14)
axs[0].grid(True)
# set tick font size 
for label in (axs[0].get_xticklabels() + axs[0].get_yticklabels()):
    label.set_fontsize(16)

axs[1].set_title("Pessimistic Constraint", fontsize=20)
axs[1].set_xlabel("Bandwidth Values", fontsize=16)
axs[1].set_ylabel("Blocking Probability", fontsize=16)
axs[1].set_xticks(range(2, 21, 2), fontsize=16)
axs[1].legend()
axs[1].grid(True)
# set tick font size
for label in (axs[1].get_xticklabels() + axs[1].get_yticklabels()):
    label.set_fontsize(16)

# Show the plot
plt.tight_layout()
plt.savefig('./fig2a.png')

plt.clf()


# do same for NFSNET

# Set up the figure and subplots
fig, axs = plt.subplots(1, 2, figsize=(14, 6))

# Plot Optimistic Results
for connections in df[(df['topology'] == 'NFSNET') & (df['type'] == 'opti')]['num_connections'].unique():
    subset = df[(df['topology'] == 'NFSNET') & (df['type'] == 'opti') & (df['num_connections'] == connections)]
    axs[0].plot(subset.columns[3:], 1-subset.iloc[0, 3:], marker='o', label=f"{subset['topology'].values[0]} - {connections} connections")

# Plot Pessimistic Results
for connections in df[(df['topology'] == 'NFSNET') & (df['type'] == 'pessi')]['num_connections'].unique():
    subset = df[(df['topology'] == 'NFSNET') & (df['type'] == 'pessi') & (df['num_connections'] == connections)]
    axs[1].plot(subset.columns[3:], 1-subset.iloc[0, 3:], marker='o', label=f"{subset['topology'].values[0]} - {connections} connections")

# Labels and Titles
axs[0].set_title("Optimistic Constraint", fontsize=20)
axs[0].set_xlabel("Bandwidth Values", fontsize=16)
axs[0].set_ylabel("Blocking Probability", fontsize=16)
axs[0].set_xticks(range(2, 21, 2), fontsize=16)
axs[0].legend(fontsize=16)
axs[0].grid(True)
# set tick font size 
for label in (axs[0].get_xticklabels() + axs[0].get_yticklabels()):
    label.set_fontsize(16)

axs[1].set_title("Pessimistic Constraint", fontsize=20)
axs[1].set_xlabel("Bandwidth Values", fontsize=16)
axs[1].set_ylabel("Blocking Probability", fontsize=16)
axs[1].set_xticks(range(2, 21, 2), fontsize=16)
axs[1].legend(fontsize=14)
axs[1].grid(True)
# set tick font size
for label in (axs[1].get_xticklabels() + axs[1].get_yticklabels()):
    label.set_fontsize(16)

# Show the plot
plt.tight_layout()
plt.savefig('./fig2b.png')