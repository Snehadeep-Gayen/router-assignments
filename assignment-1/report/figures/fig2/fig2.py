import matplotlib.pyplot as plt
import csv
import numpy as np

# Read the data from the file
arpanet_100_opti = []
arpanet_100_pessi = []
arpanet_200_opti = []
arpanet_200_pessi = []
arpanet_300_opti = []
arpanet_300_pessi = []

nsfnet_100_opti = []
nsfnet_100_pessi = []
nsfnet_200_opti = []
nsfnet_200_pessi = []
nsfnet_300_opti = []
nsfnet_300_pessi = []

with open('./fig2.csv', 'r') as file:

    reader = csv.reader(file)

    for row in reader:
        if row[0] == 'ARPANET':
            if row[1] == '100':
                if row[2] == 'optimistic':
                    arpanet_100_opti = list(map(float, row[3:]))
                else:
                    arpanet_100_pessi = list(map(float, row[3:]))
            elif row[1] == '200':
                if row[2] == 'optimistic':
                    arpanet_200_opti = list(map(float, row[3:]))
                else:
                    arpanet_200_pessi = list(map(float, row[3:]))
            else:
                if row[2] == 'optimistic':
                    arpanet_300_opti = list(map(float, row[3:]))
                else:
                    arpanet_300_pessi = list(map(float, row[3:]))
        else:
            if row[1] == '100':
                if row[2] == 'optimistic':
                    nsfnet_100_opti = list(map(float, row[3:]))
                else:
                    nsfnet_100_pessi = list(map(float, row[3:]))
            elif row[1] == '200':
                if row[2] == 'optimistic':
                    nsfnet_200_opti = list(map(float, row[3:]))
                else:
                    nsfnet_200_pessi = list(map(float, row[3:]))
            else:
                if row[2] == 'optimistic':
                    nsfnet_300_opti = list(map(float, row[3:]))
                else:
                    nsfnet_300_pessi = list(map(float, row[3:]))

# convert to numpy arrays
arpanet_100_opti = np.array(arpanet_100_opti)
arpanet_100_pessi = np.array(arpanet_100_pessi)
arpanet_200_opti = np.array(arpanet_200_opti)
arpanet_200_pessi = np.array(arpanet_200_pessi)
arpanet_300_opti = np.array(arpanet_300_opti)
arpanet_300_pessi = np.array(arpanet_300_pessi)

nsfnet_100_opti = np.array(nsfnet_100_opti)
nsfnet_100_pessi = np.array(nsfnet_100_pessi)
nsfnet_200_opti = np.array(nsfnet_200_opti)
nsfnet_200_pessi = np.array(nsfnet_200_pessi)
nsfnet_300_opti = np.array(nsfnet_300_opti)
nsfnet_300_pessi = np.array(nsfnet_300_pessi)

# Set up the figure and subplots
fig, axs = plt.subplots(1, 2, figsize=(14, 6))

# Plot Optimistic Results
axs[0].plot(range(2, 21, 2), 1-arpanet_100_opti, marker='o', label="ARPANET - 100 connections")
axs[0].plot(range(2, 21, 2), 1-arpanet_200_opti, marker='o', label="ARPANET - 200 connections")
axs[0].plot(range(2, 21, 2), 1-arpanet_300_opti, marker='o', label="ARPANET - 300 connections")

# Plot Pessimistic Results
axs[1].plot(range(2, 21, 2), 1-arpanet_100_pessi, marker='o', label="ARPANET - 100 connections")
axs[1].plot(range(2, 21, 2), 1-arpanet_200_pessi, marker='o', label="ARPANET - 200 connections")
axs[1].plot(range(2, 21, 2), 1-arpanet_300_pessi, marker='o', label="ARPANET - 300 connections")

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
axs[0].plot(range(2, 21, 2), 1-nsfnet_100_opti, marker='o', label="NFSNET - 100 connections")
axs[0].plot(range(2, 21, 2), 1-nsfnet_200_opti, marker='o', label="NFSNET - 200 connections")
axs[0].plot(range(2, 21, 2), 1-nsfnet_300_opti, marker='o', label="NFSNET - 300 connections")

# Plot Pessimistic Results
axs[1].plot(range(2, 21, 2), 1-nsfnet_100_pessi, marker='o', label="NFSNET - 100 connections")
axs[1].plot(range(2, 21, 2), 1-nsfnet_200_pessi, marker='o', label="NFSNET - 200 connections")
axs[1].plot(range(2, 21, 2), 1-nsfnet_300_pessi, marker='o', label="NFSNET - 300 connections")

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