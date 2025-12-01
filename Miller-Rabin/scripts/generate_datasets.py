#!/usr/bin/env python3
"""
generate_datasets.py
Generates CSV datasets for primality testing benchmarks.

Requires: sympy
Install with: pip install sympy
"""

import csv
from sympy import isprime, nextprime, prevprime

def generate_carmichael_dataset():
    """
    Generates dataset_carmichael.csv with exactly 5 Carmichael numbers for focused analysis.
    """
    # Select 5 well-known Carmichael numbers of varying sizes
    carmichael_numbers = [
        561,      # 3 × 11 × 17
        1105,     # 5 × 13 × 17
        1729,     # 7 × 13 × 19 (Hardy-Ramanujan number)
        2465,     # 5 × 17 × 29
        6601      # 7 × 23 × 41
    ]
    
    with open('data/dataset_carmichael.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Number', 'Type'])
        
        for num in carmichael_numbers:
            writer.writerow([num, 'Carmichael'])
    
    print(f"✓ Generated data/dataset_carmichael.csv ({len(carmichael_numbers)} Carmichael numbers)")


def generate_naive_dataset():
    """
    Generates dataset_naive.csv with primes from 20 to 50 bits (step 2).
    5 primes per bit length.
    """
    data = []
    
    for bit_length in range(20, 51, 2):
        # Calculate the range for this bit length
        min_val = 2 ** (bit_length - 1)
        max_val = 2 ** bit_length - 1
        
        # Find 5 primes in this range
        current = min_val
        count = 0
        while count < 5:
            prime = nextprime(current)
            if prime <= max_val:
                data.append([prime, bit_length])
                count += 1
                current = prime
            else:
                break
    
    with open('data/dataset_naive.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Number', 'BitLength'])
        writer.writerows(data)
    
    print(f"✓ Generated data/dataset_naive.csv ({len(data)} entries)")


def generate_mr_dataset():
    """
    Generates dataset_mr.csv with primes from 20 to 64 bits (step 4).
    5 primes per bit length.
    """
    data = []
    
    for bit_length in range(20, 65, 4):
        # Calculate the range for this bit length
        min_val = 2 ** (bit_length - 1)
        max_val = 2 ** bit_length - 1
        
        # Find 5 primes in this range
        current = min_val
        count = 0
        while count < 5:
            prime = nextprime(current)
            if prime <= max_val:
                data.append([prime, bit_length])
                count += 1
                current = prime
            else:
                break
    
    with open('data/dataset_mr.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Number', 'BitLength'])
        writer.writerows(data)
    
    print(f"✓ Generated data/dataset_mr.csv ({len(data)} entries)")


def generate_k_scaling_dataset():
    """
    Generates dataset_k_scaling.csv with a single large 64-bit prime
    repeated 50 times with k values from 1 to 50.
    """
    # Find a large 64-bit prime
    min_64bit = 2 ** 63
    large_prime = nextprime(min_64bit + 123456789)  # Offset for variety
    
    data = []
    for k in range(1, 51):
        data.append([large_prime, k])
    
    with open('data/dataset_k_scaling.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Number', 'k_value'])
        writer.writerows(data)
    
    print(f"✓ Generated data/dataset_k_scaling.csv (Prime: {large_prime}, k=1..50)")


if __name__ == '__main__':
    print("Generating datasets for primality testing benchmarks...\n")
    generate_carmichael_dataset()
    generate_naive_dataset()
    generate_mr_dataset()
    generate_k_scaling_dataset()
    print("\n✓ All datasets generated successfully!")
