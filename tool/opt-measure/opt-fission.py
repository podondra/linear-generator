#!/usr/bin/env python3

import matplotlib.pyplot as plt

n1 = []
t1 = []
with open('opt-interchange.sh.o', 'r') as f:
    for line in f:
        data = line.split()
        n1.append(int(data[0]))
        t1.append(float(data[1]))

n2 = []
t2 = []
with open('opt-popcount.sh.o', 'r') as f:
    for line in f:
        data = line.split()
        n2.append(int(data[0]))
        t2.append(float(data[1]))

n3 = []
t3 = []
with open('opt-fission.sh.o', 'r') as f:
    for line in f:
        data = line.split()
        n3.append(int(data[0]))
        t3.append(float(data[1]))

y1 = []
y2 = []
y3 = []
for i in range(len(t1)):
    y1.append(n1[i] / t1[i])
    y2.append(n2[i] / t2[i])
    y3.append(n3[i] / t3[i])

# plot data
plt.plot(n1, y1, '.-', label='loop interchange, gcc -03')
plt.plot(n2, y2, '.-', label='population count, gcc -03')
plt.plot(n3, y3, '.-', label='loop fission, gcc -03')
plt.legend(loc='best')
plt.ylabel('n / t [s ^ -1]')
plt.xlabel('pocet linearnich generatoru')
plt.title('loop fission')
plt.grid(True)
plt.show()
