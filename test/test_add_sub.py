#!/usr/bin/env python3

import sys

count = 0
add_wrong = 0
sub_wrong = 0

lines = sys.stdin.readlines()
data = [(x.strip() for x in line.split()) for line in lines]
for dd in data:
    sa, op, sb, eq, sc = dd
    a = int(sa, 16)
    b = int(sb, 16)
    c = int(sc, 16)
    l = len(sa) - 2
    count += 1
    if op == "+":
        cc = a + b
        if c != cc:
            add_wrong += 1
            print("\n{}\n+\n{}\nwrong={}\nright={:#0{}x}\n".format(sa,sb,sc,cc,l))
    elif op == "-":
        cc = a - b
        if c != cc:
            sub_wrong += 1
            print("\n{}\n-\n{}\nwrong={}\nright={:#0{}x}\n".format(sa,sb,sc,cc,l))

print("\nall:{},add_wrong:{},{:f}, sub_wrong:{},{:f}".format(count, add_wrong, add_wrong/count, sub_wrong, sub_wrong/count))
