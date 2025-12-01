#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include "miller_rabin.hpp"  
#include "trial_division.hpp"

// Utility: Get bit count
int get_bit_count(uint64_t n) {
    if (n <= 1) return 1;
    int bits = 0;
    while (n > 0) {
        bits++;
        n >>= 1;
    }
    return bits;
}

// Mode 1: Benchmark Trial Division on dataset_naive.csv
void bench_naive(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    
    // Output CSV header
    printf("Number,BitLength,TimeSec\n");
    
    std::string line;
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string num_str, bits_str;
        
        if (std::getline(ss, num_str, ',') && std::getline(ss, bits_str, ',')) {
            uint64_t number = std::stoull(num_str);
            int bit_length = std::stoi(bits_str);
            
            // Time the trial division
            clock_t start = clock();
            trial_division(number);
            clock_t end = clock();
            
            double time_sec = (double)(end - start) / CLOCKS_PER_SEC;
            
            printf("%" PRIu64 ",%d,%.10f\n", number, bit_length, time_sec);
        }
    }
    file.close();
}

// Mode 2: Benchmark Miller-Rabin (k=5) on dataset_mr.csv
void bench_mr(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    
    printf("Number,BitLength,TimeSec\n");
    
    std::string line;
    std::getline(file, line); // Skip header
    
    const int ITERATIONS = 1000; // Average over multiple runs for precision
    
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string num_str, bits_str;
        
        if (std::getline(ss, num_str, ',') && std::getline(ss, bits_str, ',')) {
            uint64_t number = std::stoull(num_str);
            int bit_length = std::stoi(bits_str);
            
            // Time Miller-Rabin with averaging
            clock_t start = clock();
            for (int i = 0; i < ITERATIONS; i++) {
                is_prime_miller_rabin(number, 5);
            }
            clock_t end = clock();
            
            double time_sec = ((double)(end - start) / CLOCKS_PER_SEC) / ITERATIONS;
            
            printf("%" PRIu64 ",%d,%.10f\n", number, bit_length, time_sec);
        }
    }
    file.close();
}

// Mode 3: Error test on Carmichael numbers
void error_test(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    
    printf("Number,Type,k,FalsePositives,TotalTrials,FalsePositiveRate\n");
    
    std::string line;
    std::getline(file, line); // Skip header
    
    const int TRIALS = 10000; // Number of trials per (number, k) pair
    
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string num_str, type_str;
        
        if (std::getline(ss, num_str, ',') && std::getline(ss, type_str, ',')) {
            uint64_t number = std::stoull(num_str);
            
            // Trim any whitespace from type_str
            type_str.erase(type_str.find_last_not_of(" \n\r\t") + 1);
            
            // Only test composites (Carmichael or regular)
            // Skip if it's actually prime (safety check)
            if (trial_division(number)) {
                continue; // Skip primes
            }
            
            // Test with k from 1 to 10
            for (int k = 1; k <= 10; k++) {
                int false_positives = 0;
                
                for (int trial = 0; trial < TRIALS; trial++) {
                    if (is_prime_miller_rabin(number, k)) {
                        false_positives++;
                    }
                }
                
                double fpr = (double)false_positives / TRIALS;
                printf("%" PRIu64 ",%s,%d,%d,%d,%.10f\n", 
                       number, type_str.c_str(), k, false_positives, TRIALS, fpr);
            }
        }
    }
    file.close();
}

// Mode 4: K-scaling test
void k_test(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    
    printf("Number,k,TimeSec\n");
    
    std::string line;
    std::getline(file, line); // Skip header
    
    const int ITERATIONS = 100; // Average for precision
    
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string num_str, k_str;
        
        if (std::getline(ss, num_str, ',') && std::getline(ss, k_str, ',')) {
            uint64_t number = std::stoull(num_str);
            int k = std::stoi(k_str);
            
            // Time Miller-Rabin with this k value
            clock_t start = clock();
            for (int i = 0; i < ITERATIONS; i++) {
                is_prime_miller_rabin(number, k);
            }
            clock_t end = clock();
            
            double time_sec = ((double)(end - start) / CLOCKS_PER_SEC) / ITERATIONS;
            
            printf("%" PRIu64 ",%d,%.10f\n", number, k, time_sec);
        }
    }
    file.close();
}

void print_usage() {
    std::cerr << "Usage:\n";
    std::cerr << "  ./main_batch --bench-naive <file.csv>   # Benchmark Trial Division\n";
    std::cerr << "  ./main_batch --bench-mr <file.csv>      # Benchmark Miller-Rabin (k=5)\n";
    std::cerr << "  ./main_batch --error-test <file.csv>    # Error analysis on composites\n";
    std::cerr << "  ./main_batch --k-test <file.csv>        # K-scaling analysis\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }
    
    const char* mode = argv[1];
    const char* filename = argv[2];
    
    if (strcmp(mode, "--bench-naive") == 0) {
        bench_naive(filename);
    } else if (strcmp(mode, "--bench-mr") == 0) {
        bench_mr(filename);
    } else if (strcmp(mode, "--error-test") == 0) {
        error_test(filename);
    } else if (strcmp(mode, "--k-test") == 0) {
        k_test(filename);
    } else {
        std::cerr << "Error: Unknown mode '" << mode << "'\n";
        print_usage();
        return 1;
    }
    
    return 0;
}
