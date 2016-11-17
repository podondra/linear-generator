#!/usr/bin/env python3

import matplotlib.pyplot as plt

n = []
t = []
with open('seq-long.sh.o84746', 'r') as f:
    for line in f:
        d = line.split()
        n.append(int(d[0]))
        t.append(float(d[1]))

plt.title('casova slozitost sekvenci implementace')

plt.plot(n, time, 'b.-', label='gcc -O3')

plt.legend(loc='best')
plt.ylabel('cas [s]')
plt.xlabel('pocet G')
plt.grid(True)

plt.savefig('seq.svg')
