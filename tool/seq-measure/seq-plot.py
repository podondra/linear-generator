#!/usr/bin/env python3

import matplotlib.pyplot as plt

n = []
time = []

# read input data
with open('seq-long.sh.o', 'r') as f:
    for line in f:
        data = line.split()
        n.append(int(data[0]))
        time.append(float(data[1]))

# plot data
plt.plot(n, time, '.-', label='gcc -03')
plt.legend(loc='best')
plt.ylabel('cas [s]')
plt.xlabel('pocet linearnich generatoru')
plt.title('casova slozitost sekvenci implementace')
plt.grid(True)
plt.show()
