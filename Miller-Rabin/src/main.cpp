#include <cstdio>    // C++ version of stdio.h (printf)
#include <ctime>     // C++ version of time.h (clock)
#include <cinttypes> // For PRIu64 macros (portable uint64_t printing)
#include <cstdint>   // For uint64_t
#include <iostream>  // For user input (cin, cout)
#include <vector>    // For dynamic test case array
#include <random>    // For random number generation
#include <cmath>     // For log2 and floor
#include <limits>    // For numeric_limits
#include <algorithm> // For std::max

// Local includes for your project files
#include "miller_rabin.hpp"  
#include "trial_division.hpp"

// Function to run the error analysis experiment
void run_error_analysis(uint64_t N) {
    const uint32_t TOTAL_TRIALS = 500000; // Run half a million times for robust statistics

    printf("\n\n=== EMPIRICAL ERROR RATE ANALYSIS ===\n");
    printf("Testing number N = %" PRIu64 " (%u trials per k)\n", N, TOTAL_TRIALS);
    printf("Theoretical Max Error (FPR) for a composite is 4^-k\n");
    printf("-------------------------------------------------------------------------\n");
    printf("%-5s | %-12s | %-15s | %-10s\n", "k", "Expected FPR", "False Positives", "Actual FPR");
    printf("-------------------------------------------------------------------------\n");

    // Test for k = 1, 2, 3, 4
    for (int k = 1; k <= 4; k++) {
        uint32_t false_positives = 0;
        
        // Run the Miller-Rabin test T times for the fixed k
        for (uint32_t i = 0; i < TOTAL_TRIALS; i++) {
            if (is_prime_miller_rabin(N, k)) {
                false_positives++;
            }
        }

        // Calculate the results
        double expected_fpr = 1.0 / (1ULL << (2 * k)); // 4^-k or 1/(2^2k)
        double actual_fpr = (double)false_positives / TOTAL_TRIALS;

        // Print the results for the report
        printf("%-5d | %-12.8f | %-15u | %-10.8f\n", 
               k, 
               expected_fpr, 
               false_positives, 
               actual_fpr);
    }
    printf("-------------------------------------------------------------------------\n");
}

// Structure to hold our test cases
typedef struct {
    uint64_t val;
    int bits;
} TestCase;

// Function to get the bit count of a number
int get_bit_count(uint64_t n) {
    if (n <= 1) return 1;
    // Use the maximum of 1 or the calculated bits
    return (int)std::floor(std::log2((double)n)) + 1;
}

// Overloaded function to run the speed benchmark with a given set of test cases
void run_speed_benchmark(const std::vector<TestCase>& tests) {
    printf("\n[Speed Benchmark] Comparing Execution Time\n");
    printf("%-20s | %-10s | %-20s | %-20s | %s\n", 
           "Number", "Bits", "Trial Div (sec)", "Miller-Rabin (sec)", "Match?");
    printf("--------------------------------------------------------------------------------------------\n");

    const int MR_AVERAGE_ITERATIONS = 1000; // MUST average due to low clock resolution
    
    for (const auto& test : tests) {
        uint64_t n = test.val;
        
        // Skip invalid inputs that would cause errors in Trial Division (e.g., n=1)
        if (n <= 1) {
             printf("%-20" PRIu64 " | %-10d | Invalid Input (<=1)\n", n, test.bits);
             continue;
        }

        // --- Time Trial Division (Single Run) ---
        // Note: Using the correct function name: trial_division
        clock_t start_td = clock();
        bool is_prime_td = trial_division(n); 
        clock_t end_td = clock();
        double time_td = (double)(end_td - start_td) / CLOCKS_PER_SEC;

        // --- Time Miller-Rabin (Averaged Run) ---
        // MUST loop to get measurable time!
        clock_t start_mr = clock();
        bool is_prime_mr = false;
        for (int i = 0; i < MR_AVERAGE_ITERATIONS; ++i) {
             is_prime_mr = is_prime_miller_rabin(n, 5); 
        }
        clock_t end_mr = clock();
        // Divide by the number of iterations to get the average time per run
        double time_mr = ((double)(end_mr - start_mr) / CLOCKS_PER_SEC) / MR_AVERAGE_ITERATIONS;


        // --- Print Results ---
        printf("%-20" PRIu64 " | %-10d | %-20.8f | %-20.8f | %s\n",
               n,
               test.bits,
               time_td,
               time_mr,
               (is_prime_td == is_prime_mr) ? "Yes" : "No");
    }
    printf("--------------------------------------------------------------------------------------------\n");
}


int main() {
    // Note: We rely on the Mersenne Twister engine being seeded in miller_rabin.cpp,
    // so std::srand(time(NULL)) is not strictly necessary but harmless.

    while (true) {
        int main_choice;
        
        // Input cleanup for robust menu handling
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "\n=== Primality Testing Analysis ===\n";
        std::cout << "Select the test you want to run:\n";
        std::cout << "1. Speed Benchmark (Miller-Rabin vs. Trial Division)\n";
        std::cout << "2. Miller-Rabin Empirical Error Rate Analysis\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        
        if (!(std::cin >> main_choice)) {
            // Handle Ctrl+D or non-integer input
            break;
        }
        
        // Clean up input buffer after successful read
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        if (main_choice == 1) {
            int speed_choice;
            std::cout << "\n--- Speed Benchmark Setup ---\n";
            std::cout << "1. Run with hardcoded test cases (default)\n";
            std::cout << "2. Enter custom test cases\n";
            std::cout << "3. Generate random test cases in a range\n";
            std::cout << "Enter your choice: ";
            
            if (!(std::cin >> speed_choice)) continue;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


            std::vector<TestCase> tests;
            
            // --- Option 2: Custom Cases ---
            if (speed_choice == 2) {
                int n_custom;
                std::cout << "How many numbers do you want to test? ";
                if (!(std::cin >> n_custom) || n_custom <= 0 || n_custom > 100) {
                     std::cerr << "Invalid count. Max 100.\n"; continue;
                }
                tests.resize(n_custom);
                std::cout << "Enter the " << n_custom << " numbers (64-bit unsigned):\n";
                for (int i = 0; i < n_custom; ++i) {
                    std::cout << "> ";
                    uint64_t val;
                    // Use standard C-style scanf for portable uint64_t reading in C++
                    if (scanf("%" PRIu64, &val) != 1) {
                         std::cerr << "Error reading value. Stopping input.\n"; break;
                    }
                    tests[i].val = val;
                    tests[i].bits = get_bit_count(val);
                }
                
            // --- Option 3: Random Range ---
            } else if (speed_choice == 3) {
                int n_random;
                uint64_t min_val, max_val;
                std::cout << "How many random numbers to generate? ";
                if (!(std::cin >> n_random) || n_random <= 0 || n_random > 100) {
                    std::cerr << "Invalid count. Max 100.\n"; continue;
                }
                std::cout << "Enter the minimum 64-bit value: ";
                if (!(std::cin >> min_val)) continue;
                std::cout << "Enter the maximum 64-bit value: ";
                if (!(std::cin >> max_val)) continue;

                if (min_val > max_val) {
                    std::cerr << "Error: Minimum value cannot be greater than maximum value.\n";
                    std::swap(min_val, max_val); // Swap for robustness
                    std::cerr << "Swapping range to [" << min_val << ", " << max_val << "].\n";
                }
                
                // Use the random generation function exposed from the Miller-Rabin file
                tests.reserve(n_random);
                std::cout << "Generating " << n_random << " random numbers...\n";
                for (int i = 0; i < n_random; ++i) {
                    uint64_t val = get_random_range_public(min_val, max_val);
                    // Ensure odd numbers are tested (if > 2) for non-trivial testing
                    if (val > 2 && val % 2 == 0) { 
                        val++; 
                        if (val > max_val) val -= 2; // Keep in range
                    }
                    tests.push_back({val, get_bit_count(val)});
                }

            // --- Option 1: Hardcoded Defaults ---
            } else {
                std::cout << "Running with hardcoded default test cases...\n";
                tests = {
                    {65521ULL, 16},                  
                    {4294967291ULL, 32},             
                    {1099511627689ULL, 40},          
                    {281474976710597ULL, 48},        
                    {18014398509481951ULL, 54}       
                };
            }
            run_speed_benchmark(tests);

        } else if (main_choice == 2) {
            // --- Error Analysis ---
            uint64_t N_error = 0;
            int error_choice;

            std::cout << "\n--- Miller-Rabin Error Analysis Setup ---\n";
            std::cout << "1. Use hardcoded Carmichael number (1729)\n";
            std::cout << "2. Enter a custom composite number\n";
            std::cout << "Enter your choice: ";
            if (!(std::cin >> error_choice)) continue;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (error_choice == 1) {
                N_error = 1729;
            } else if (error_choice == 2) {
                std::cout << "Enter the composite number N: ";
                if (!(std::cin >> N_error)) continue;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                std::cerr << "Invalid choice for error analysis.\n";
                continue;
            }

            if (N_error <= 1) {
                std::cerr << "Error: Number must be > 1.\n";
            } else if (trial_division(N_error)) {
                std::cerr << "Warning: The chosen number " << N_error << " is prime.\n";
                std::cerr << "The analysis will run, but cannot find false positives for a prime number (FPR should be 0).\n";
                run_error_analysis(N_error);
            } else {
                // Number is composite or prime but we run the analysis anyway
                run_error_analysis(N_error);
            }


        } else if (main_choice == 3) {
            std::cout << "Exiting program.\n";
            break;
        } else {
            std::cerr << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}