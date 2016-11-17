#!/usr/bin/env python3

import matplotlib.pyplot as plt

# read seq input data
n_seq = []
time_seq = []
with open('../seq-measure/seq-long.sh.o84518', 'r') as f:
    for line in f:
        data = line.split()
        n_seq.append(int(data[0]))
        time_seq.append(float(data[1]))

# read opt input data
n_opt = []
time_opt = []

with open('opt-popcount-long.sh.o84557', 'r') as f:
    for line in f:
        data = line.split()
        n_opt.append(int(data[0]))
        time_opt.append(float(data[1]))

op_seq = []
op_opt = []
for i in range(len(time_seq)):
    op_seq.append(n_seq[i] / time_seq[i])
    op_opt.append(n_opt[i] / time_opt[i])

# plot data
plt.plot(n_seq, op_seq, '.-', label='sekvenci, gcc -03')
plt.plot(n_opt, op_opt, '.-', label='population count, gcc -03')
plt.legend(loc='best')
plt.ylabel('n / t [s ^ -1]')
plt.xlabel('pocet linearnich generatoru')
plt.title('linearnich generatoru za sekundu - population count')
plt.grid(True)
plt.show()
