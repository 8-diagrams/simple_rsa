#!/usr/bin/env python3

import sys

class tester():
  def __init__(self, op):
    self._count = 0
    self._wrong = 0
    self._op = op
    if op == "mod_mul_inv":
      self._test_ = self.mod_mul_inv
    else:
      self._test_ = lambda _: False

  def mod_mul_inv(self, a, b, c):
    return b * c % a == 1

  def test(self, argv):
    self._count += 1
    args = [int(x,16) for x in argv.split()]
    r = self._test_(*args)
    if not r:
      self._wrong += 1
      print("\n{}\n{}\n".format(self._op, argv))

  def report(self):
    print("{} -- all:{}".format(self._op, self._count))
    if self._count > 0:
      print("wrong: {}, {:3f}".format(self._wrong, self._wrong/self._count))
    print()


_ops = ["mod_mul_inv"]
_testers = {op : tester(op) for op in _ops}

for line in sys.stdin.readlines():
  op, argv = line.split(" ", 1)
  _testers[op.strip()].test(argv)

for t in _testers:
  _testers[t].report()
