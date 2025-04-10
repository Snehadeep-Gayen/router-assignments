import numpy as np

import matplotlib.pyplot as plt

data = np.array([[0.97,0.87,0.73], [0.98,0.73,0.65]])
labels = ['ARPANET', 'NFSNET']
x = np.arange(len(labels))
width = 0.35

fig, ax = plt.subplots()
# draw gridlines in the background
ax.set_axisbelow(True)
ax.yaxis.grid(color='gray', linestyle='dashed')

# draw bars with the number on top
ax.bar(x - width/2, 1-data[:, 0], width/2, label='100 connections', color='brown')
ax.bar(x, 1-data[:, 1], width/2, label='200 connections', color='purple')
ax.bar(x + width/2, 1-data[:, 2], width/2, label='300 connections', color='orange')

# rects1 = ax.bar(x - width/2, data[:, 0], width/2, label='100')
# rects2 = ax.bar(x, data[:, 1], width/2, label='200')
# rects3 = ax.bar(x + width/2, data[:, 2], width/2, label='300')

ax.set_ylabel('Blocking Probability of a Connection')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()

fig.tight_layout()

plt.savefig('/home/snehadeep/Documents/router/router-assignments/assignment-1/report/figures/fig1/fig1.png')