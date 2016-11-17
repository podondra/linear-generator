#!/usr/bin/env python3

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
    print(n_seq[i], time_seq[i], time_opt[i], round(time_seq[i] / time_opt[i], 6))
    tmp = tmp + (time_seq[i] / time_opt[i])

print(tmp / len(time_seq))
