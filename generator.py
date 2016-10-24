import random
import sys

num = int(sys.argv[1])

print(num)

while num > 0:
    num = num - 1
    n = random.randint(11, 31)
    a = random.choice(range(1, 2 ** n, 2));
    b = random.choice(range(1, 2 ** n, 2));
    print(a, b, n)
