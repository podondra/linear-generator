# LINEARNI GENERATORY

TODO translation

Mejme nekolik (_G_, pozor muze to byt i cislo v radech milionu) danych
linearnich generatoru, kazdy z nich je dan parametry _a_, _b_, _n_. Generator
vypocitava posloupnost _x[i] = (a * x[i - 1] + b) mod 2 ^ n_, kde _a_ a _b_ jsou
kladna __licha__ cisla, _10 < n < 32_ a _x[0] = 0_. Pocet clenu teto
posloupnosti je _k_ (pro vsechny generatory stejne). Ukolem je pro dane
konstanty _c_, _d_, _e_ (pro vsechny generatory stejne), najit:

- kolikrat je pro dany generator _x[i]_ v intervalu _[c, d]_
- kolik je pro dany generator minimálni a maximalni Hammingova vzdalenost mezi
  _x[i]_ a parametrem _e_

## UKOL

- loop unrolling
- loop tiling
- vektorizace
