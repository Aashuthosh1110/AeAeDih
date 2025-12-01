/*
 * benchmark_suite.c
 * - Runs Bubble/Merge Sort up to N=20
 * - Runs Bogosort up to N=13
 * - Trials: 20 for small N, 5 for large N
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- Sorting Algorithms ---
void bubble_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void merge(int *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(int *arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// --- Helpers ---
void generate_array(int *arr, int n) {
    for(int i=0; i<n; i++) arr[i] = rand() % 100;
}

void copy_array(int *src, int *dest, int n) {
    for(int i=0; i<n; i++) dest[i] = src[i];
}

int main() {
    srand(time(NULL));
    FILE *csv = fopen("benchmark_results.csv", "w");
    if (!csv) { perror("File error"); return 1; }
    
    fprintf(csv, "N,BubbleSort_Time,MergeSort_Time,Bogosort_Time\n");
    
    // Bubble/Merge go to 20. Bogosort stops at 13.
    int MAX_N = 20; 
    int MAX_BOGO_N = 13;

    printf("Starting Benchmarks (Bubble/Merge to %d, Bogo to %d)...\n", MAX_N, MAX_BOGO_N);

    for (int n = 2; n <= MAX_N; n++) {
        // 20 trials for small N to smooth variance
        // 5 trials for large N to save time
        int trials = (n <= 10) ? 20 : 5;
        
        printf("Benchmarking N=%d (%d trials)... ", n, trials);
        fflush(stdout);

        int *master_arr = malloc(sizeof(int) * n);
        int *temp_arr = malloc(sizeof(int) * n);
        
        double total_bubble = 0;
        double total_merge = 0;
        double total_bogo = 0;

        for (int t = 0; t < trials; t++) {
            generate_array(master_arr, n);

            // 1. Bubble Sort
            copy_array(master_arr, temp_arr, n);
            clock_t start = clock();
            bubble_sort(temp_arr, n);
            total_bubble += (double)(clock() - start) / CLOCKS_PER_SEC;

            // 2. Merge Sort
            copy_array(master_arr, temp_arr, n);
            start = clock();
            merge_sort(temp_arr, 0, n-1);
            total_merge += (double)(clock() - start) / CLOCKS_PER_SEC;

            // 3. Bogosort (Only run if N <= 13)
            if (n <= MAX_BOGO_N) {
                char cmd[8192]; 
                char num[64];   
                sprintf(cmd, "./bogosort_cli");
                for(int i=0; i<n; i++) {
                    sprintf(num, " %d", master_arr[i]);
                    strcat(cmd, num);
                }

                FILE *fp = popen(cmd, "r");
                if (fp == NULL) { printf("Err\n"); exit(1); }

                char result_buffer[1024];
                while (fgets(result_buffer, sizeof(result_buffer), fp) != NULL) {
                    if (strncmp(result_buffer, "RESULT:", 7) == 0) {
                        char *token = strtok(result_buffer, ":"); 
                        token = strtok(NULL, ":"); // Array
                        token = strtok(NULL, ":"); // Shuffles
                        token = strtok(NULL, ":"); // Time
                        if(token) total_bogo += atof(token);
                    }
                }
                pclose(fp);
            }
        }

        double avg_bubble = total_bubble / trials;
        double avg_merge = total_merge / trials;
        double avg_bogo = (n <= MAX_BOGO_N) ? (total_bogo / trials) : 0.0;

        printf("Done.\n");
        fprintf(csv, "%d,%.9f,%.9f,%.9f\n", n, avg_bubble, avg_merge, avg_bogo);
        
        free(master_arr);
        free(temp_arr);
    }

    fclose(csv);
    printf("Benchmark complete. Data saved to benchmark_results.csv\n");
    return 0;
}