/* * bogosort_cli.c
 * Standalone executable.
 * Usage: ./bogosort_cli 5 2 9 1
 * Output: CSV-style statistics for the parent process to read.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Fisher-Yates Shuffle
void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

bool is_sorted(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        printf("Usage: %s <num1> <num2> ...\n", argv[0]);
        return 1;
    }

    int n = argc - 1;
    int *arr = malloc(sizeof(int) * n);
    
    // Parse command line args
    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    clock_t start = clock();
    long long shuffles = 0;

    while (!is_sorted(arr, n)) {
        shuffle(arr, n);
        shuffles++;
    }

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    // Output format: SORTED_ARRAY | SHUFFLES | TIME
    // This specific format is parsed by the benchmark suite
    printf("RESULT:");
    for(int i=0; i<n; i++) printf("%d ", arr[i]);
    printf(":%lld:%.6f\n", shuffles, time_taken);

    free(arr);
    return 0;
}