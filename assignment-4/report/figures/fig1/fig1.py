
import matplotlib.pyplot as plt
import numpy as np


# Example data (replace with your actual data)
n8_1_ideal = [11.125] * 8
n8_1_obs = [11.125] * 8

n8_2_ideal = [20, 12, 12, 8, 6, 4, 2, 1]
n8_2_ideal = np.array(n8_2_ideal) / sum(n8_2_ideal)
n8_2_obs =  [1, 3, 6, 9, 11, 17, 17, 29]
n8_2_obs.reverse()
n8_2_obs = np.array(n8_2_obs) / sum(n8_2_obs)

n16_1_ideal = [6.25] * 16
n16_1_obs = [6.25] * 16

n16_2_ideal = [10, 10, 10, 6, 6, 6, 4, 4, 4, 2, 2, 2, 1, 1, 1, 30]
n16_2_obs = [9, 9, 9, 5, 5, 5, 3, 3, 3, 2, 2, 2, 1, 1, 1, 26]
n16_2_ideal = np.array(n16_2_ideal) / sum(n16_2_ideal)
n16_2_obs = np.array(n16_2_obs) / sum(n16_2_obs)

# List of all data pairs
data_pairs = [
    ("$N=8$ Case-1", n8_1_ideal, n8_1_obs),
    ("$N=8$ Case-2", n8_2_ideal, n8_2_obs),
    ("$N=16$ Case-1", n16_1_ideal, n16_1_obs),
    ("$N=16$ Case-2", n16_2_ideal, n16_2_obs)
]

# Plot each pair
for name, ideal, obs in data_pairs:
    indices = np.arange(len(ideal))
    assert len(ideal) == len(obs)
    width = 0.35  # Width of the bars

    plt.figure(figsize=(8, 6))
    plt.bar(indices - width/2, ideal, width, label=f'{name.split()[0]} Ideal')
    plt.bar(indices + width/2, obs, width, label=f'{name.split()[0]} Observed')

    plt.xlabel('Flow Number')
    plt.ylabel('Bandwaidth Allocation %')
    plt.title(f'{name}')
    plt.xticks(indices)
    plt.legend()

    plt.savefig(str(name.split()[0].replace('$','') + '_' + name.split('-')[-1] + '.png'))

