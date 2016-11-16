linearni generatory
===================

<podszond@fit.cvut.cz>

kapitola 1
----------

### definice problemu ###

Mějme několik (`G`, pozor může to být i číslo v řádech miliónů!) daných
lineárních generátorů, každý z nich je dán parametry `a`, `b`, `n`. Generátor
vypočítává posloupnost `x[i] = (a * x[i - 1] + b) mod 2 ^ n`, kde `a` a `b`
jsou kladná lichá čísla, `10 < n < 32` a `x[0] = 0`. Počet členů této
posloupnosti je `k` (pro všechny generátory stejné). Úkolem je pro dané
konstanty `c`, `d`, `e` (pro všechny generátory stejné), najít:

1. kolikrát je pro daný generátor `x[i]` v intervalu `[c, d]`
2. kolik je pro daný generátor minimální a maximální Hammingova vzdálenost mezi
    `x[i]` a parametrem `e`
