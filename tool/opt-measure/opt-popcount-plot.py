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

tmp = 0
for i in range(len(time_seq)):
    print(time_seq[i] / time_opt[i])
    tmp = tmp + (time_seq[i] / time_opt[i])

print(tmp / len(time_seq))

# plot data
plt.plot(n_seq, time_seq, '.-', label='sekvenci, gcc -03')
plt.plot(n_opt, time_opt, '.-', label='popcount, gcc -03')
plt.legend(loc='best')
plt.ylabel('cas [s]')
plt.xlabel('pocet linearnich generatoru')
plt.title('casova slozitost - pouziti popcount')
plt.grid(True)
plt.show()
