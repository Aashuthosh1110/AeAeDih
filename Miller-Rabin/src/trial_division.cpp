#include "trial_division.hpp"

bool trial_division(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    // Check only 6k +/- 1 up to sqrt(n)
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}