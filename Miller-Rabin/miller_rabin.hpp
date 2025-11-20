#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP

#include <cstdint> // Provides uint64_t and standard integer types
#include <cstdlib> // For 'bool' type, though cstdint is often sufficient

/**
 * Modular Exponentiation: Calculates (base^exp) % mod efficiently.
 * Uses the "Square and Multiply" algorithm and internal 128-bit casting 
 * to handle large numbers without overflow.
 */
uint64_t power(uint64_t base, uint64_t exp, uint64_t mod);

/**
 * Main Miller-Rabin Primality Test wrapper.
 * @param n The number to test.
 * @param k The number of iterations (witnesses). Higher k reduces the error rate.
 * @return true if 'n' is probably prime, false if definitely composite.
 */
bool is_prime_miller_rabin(uint64_t n, int k);

/**
 * Generates a random number in the range [min, max].
 */
uint64_t get_random_range_public(uint64_t min, uint64_t max);

#endif // MILLER_RABIN_HPP