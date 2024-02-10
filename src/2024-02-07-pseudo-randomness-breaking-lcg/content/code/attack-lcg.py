#!/usr/bin/env python3
#
# Code that shows how to attack a simple LCG PRNG by obtaining a
# series of direct outputs from the PRNG.
#
# Refs:
# - https://security.stackexchange.com/questions/4268/cracking-a-linear-congruential-generator
#
import random
import sys
import math
from functools import reduce
from lcg import LCG

# -------------------------------------

def compute_modulus(outputs):
    ts = []
    for i in range(0, len(outputs) - 1):
        ts.append(outputs[i+1] - outputs[i])
        
    us = []
    for i in range(0, len(ts)-2):
        us.append(abs(ts[i+2]*ts[i] - ts[i+1]**2))

    modulus =  reduce(math.gcd, us) #!
    return modulus

def compute_multiplier(outputs, modulus):
    term_1 = outputs[1] - outputs[2]
    term_2 = pow(outputs[0] - outputs[1], -1, modulus) #!
    a = (term_1 * term_2) % modulus
    return a

def compute_increment(outputs, modulus, a):
    b = (outputs[1] - outputs[0] * a) % modulus
    return b

# -------------------------------------    

def main():
    # prng = LCG(seed=1337, a=1337, b=42, c=444447)
    prng = LCG(seed=1338)

    n = 10
    outputs = []
    for i in range(0, n):
        outputs.append(prng.next())

    print(outputs)
    
    c = compute_modulus(outputs)
    a = compute_multiplier(outputs, c)
    b = compute_increment(outputs, c, a)

    print(f"c={c}")    
    print(f"a={a}")
    print(f"b={b}")
    
if __name__ == "__main__":
    main()
