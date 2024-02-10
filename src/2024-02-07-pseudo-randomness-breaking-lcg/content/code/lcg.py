# 
# Implementation of a basic Linear Congruential Generator, one of the
# simplests form of Pseudo-Random Number Generator.
#
# Notice that a basic LCG is not cryptographically safe, meaning that
# a malicious attacker, by observing consecutive outputs from the
# generator, is able to infer the internal state of the generator and
# thus also the next numbers the generator will produce.
#
# For this reason, you should not use this generator in security
# contexts where predictability might imply dangerous consequences.
# 
class LCG(object):
    def __init__(self, seed=1337, a=1103515245,b=12345,c=2147483648):   
        self.x = seed
        self.a = a
        self.b = b
        self.c = c

    def next(self):
        self.x = (self.x * self.a + self.b) % self.c
        return self.x
