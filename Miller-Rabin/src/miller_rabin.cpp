#include "miller_rabin.hpp"
#include <random> // High-quality random number generation (Mersenne Twister)
#include <cstdint> // For uint64_t
#include <cstdlib> // For bool

/* * GLOBAL RANDOM NUMBER GENERATOR
 * std::mt19937_64 is the 64-bit Mersenne Twister engine.
 */
static std::mt19937_64 rng(std::random_device{}());

/*
 * Helper: Get a random number in range [min, max]
 */
static uint64_t get_random_range_internal(uint64_t min, uint64_t max) {
    std::uniform_int_distribution<uint64_t> dist(min, max);
    return dist(rng);
}

/**
 * Public wrapper for the internal random function.
 */
uint64_t get_random_range_public(uint64_t min, uint64_t max) {
    return get_random_range_internal(min, max);
}

/*
 * Modular Exponentiation Implementation
 * CRITICAL: Uses `unsigned __int128` to prevent 64-bit overflow during multiplication.
 */
uint64_t power(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            // Cast to __int128 to perform safe multiplication
            res = (uint64_t)((unsigned __int128)res * base % mod);
        }
        base = (uint64_t)((unsigned __int128)base * base % mod);
        exp /= 2;
    }
    return res;
}

/*
 * Single round of the Miller-Rabin test.
 */
bool miller_rabin_single_round(uint64_t d, uint64_t n) {
    uint64_t a = get_random_range_internal(2, n - 2);
    uint64_t x = power(a, d, n);
    
    if (x == 1 || x == n - 1)
        return true;
        
    while (d != n - 1) {
        x = (uint64_t)((unsigned __int128)x * x % n);
        d *= 2;
        
        if (x == 1)      return false; 
        if (x == n - 1)  return true; 
    }
    
    return false; // Composite
}

/*
 * Main Miller-Rabin Wrapper
 */
bool is_prime_miller_rabin(uint64_t n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;
    
    // Decompose n - 1 = 2^r * d
    uint64_t d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    
    // Run k iterations
    for (int i = 0; i < k; i++) {
        if (!miller_rabin_single_round(d, n))
            return false;
    }
    
    return true; 
}