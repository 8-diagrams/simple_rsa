#!/usr/bin/env python3

import sys

class tester():
  def __init__(self, op):
    self._count = 0
    self._wrong = 0
    self._op = op
    if op == "+":
      self._op_ = lambda x, y : x+y
    elif op == "-":
      self._op_ = lambda x, y : x-y
    elif op == "*":
      self._op_ = lambda x, y : x*y
    elif op == "/":
      self._op_ = lambda x, y : x//y
    else:
      self._op_ = lambda x, y : 0

  def test(self, a, b, c):
    self._count += 1
    cc = self._op_(a, b)
    if c != cc:
      self._wrong += 1
      print("\n{:#x}\n{:#x}\n{:#x}\nwrong={:#x}\nright={:#x}\n".format(a,self._op,b,c,d))

  def report(self):
    print("{} -- all:{}".format(self._op, self._count))
    if self._count > 0:
      print("wrong: {}, {:3f}".format(self._wrong, self._wrong/self._count))
    print()


_ops = ["+", "-", "*", "/"]
_testers = {op : tester(op) for op in _ops}

for line in sys.stdin.readlines():
  sa, op, sb, eq, sc = line.split()
  a = int(sa.strip(), 16)
  b = int(sb.strip(), 16)
  c = int(sc.strip(), 16)
  _testers[op.strip()].test(a, b, c)

print("\n")

for t in _testers:
  _testers[t].report()
