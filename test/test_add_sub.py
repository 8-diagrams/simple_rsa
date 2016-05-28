#!/usr/bin/env python3

import sys

count = 0
right_add = 0
right_sub = 0

for line in sys.stdin.readlines():
    count += 1
    nums = [int(x, 16) for x in line.strip().split(",")]
    a, b, c, d = nums
    cc = (a + b) % (2**1024)
    if c == cc:
        right_add += 1
    else:
        print("\n{} +\n{} = \n{}, \n{}\n".format(hex(a), hex(b), hex(c), hex(cc)))
    dd = a - b if a >= b else b -a
    if d == dd:
        right_sub += 1
    else:
        print("\n{} -\n{} = \n{}, \n{}\n".format(hex(a), hex(b), hex(d), hex(dd)))

print("\nall:{},add right:{},{:f}, sub right:{},{:f}".format(count, right_add, right_add/count, right_sub, right_sub/count))
