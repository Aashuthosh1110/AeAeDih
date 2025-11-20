#ifndef TRIAL_DIVISION_HPP
#define TRIAL_DIVISION_HPP

#include <cstdint> 

/**
 * Deterministic Trial Division
 * Checks primality by dividing 'n' by every number up to sqrt(n).
 * Complexity: O(sqrt(n))
 */
bool trial_division(uint64_t n);

#endif // TRIAL_DIVISION_HPP