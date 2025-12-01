Comparative Analysis of Normal and Randomized QuickSort

Author: Shoaib Ahmed
Course: Advanced Algorithm and Design
Institution: — International Institute of Information Technology
Date: — 02/12/2025

Abstract

This project investigates the theoretical and empirical performance of two widely used sorting algorithms: deterministic QuickSort and randomized QuickSort. Although both algorithms share the same fundamental divide-and-conquer structure, their pivot-selection strategies lead to significant differences in performance, especially on adversarial or structured inputs. The project implements both variants in the C programming language and evaluates them across different datasets, including random arrays, sorted arrays, reverse-sorted arrays, and duplicate-heavy arrays.

An important aspect of this study concerns the choice of partitioning scheme. Preliminary tests with randomized QuickSort using the Lomuto partition method showed pathological behavior on arrays containing large numbers of repeated keys. To ensure meaningful empirical results, the implementation was adapted to use Hoare’s partition scheme, which handles duplicates more efficiently and produces partitions that better reflect the theoretical expected performance.

Experimental results include wall-clock execution times averaged over multiple runs, enabling comparison between empirical behavior and theoretical asymptotic analysis. The findings show that randomized QuickSort consistently achieves close to O(n log n) performance across all input types, while deterministic QuickSort suffers noticeable degradation on sorted and reverse-sorted inputs. The report concludes by discussing the causes of runtime variability and interpreting the results in light of algorithm design and system-level behavior.

1. Introduction

Sorting plays a central role in computer science due to its use in searching, data preparation, scheduling, and numerous system-level tasks. QuickSort is favored in practice because of its excellent average-case performance, in-place memory usage, and cache-efficient behavior. However, its performance depends critically on the choice of pivot at each recursive step.

This project studies two variants:

Normal (Deterministic) QuickSort, which chooses a fixed pivot such as the last element.

Randomized QuickSort, which selects the pivot uniformly at random.

The real-world relevance of this comparison lies in the fact that many standard libraries (including parts of the C++ STL and Java’s runtime sorting) rely on randomized or hybrid QuickSort for robustness. Understanding the behavior of these algorithms across input types is essential for algorithm designers and system programmers.

The objectives of this project are:

To compare theoretical time and space complexities of both algorithms.

To implement both versions in C and study how design decisions, such as partition schemes, affect efficiency.

To experimentally analyze behavior across various datasets and interpret discrepancies between theory and practice.

2. Algorithm Descriptions
2.1 Deterministic (Normal) QuickSort

This version selects a fixed pivot—typically the last element of the array. The algorithm partitions the array into elements smaller than the pivot and elements greater than or equal to the pivot. It then recursively sorts each partition.

Time Complexity:

Best Case: O(n log n)

Average Case: O(n log n)

Worst Case: O(n²) — occurs when input is already sorted or reverse-sorted

Space Complexity: O(log n) expected due to recursion depth; O(n) in worst case.

2.2 Randomized QuickSort

This version selects the pivot uniformly at random from the subarray. Randomization ensures that the pivot is unlikely to be consistently poor, reducing the probability of unbalanced partitions.

Time Complexity:

Expected Time: O(n log n)

Worst Case: O(n²), but highly unlikely

Space Complexity: Same as deterministic.

2.3 Partition Scheme Choice (Lomuto vs. Hoare)

Initial implementations used Lomuto partition, but it performed extremely poorly on arrays containing many repeated values. In such cases, Lomuto often produced partitions of size (n−1, 0), resulting in recursion patterns equivalent to the worst case.

To avoid this, the final implementation uses Hoare partition, which:

Moves two indices inward, swapping out-of-place elements

Handles duplicates cleanly

Produces significantly more balanced partitions

Preserves correctness with random pivot selection

This allows the measured performance of Randomized QuickSort to reflect its true expected behavior.

3. Implementation Details

Both algorithms were implemented in C, using standard arrays and recursive functions. No additional libraries were required beyond <stdlib.h> for randomization and memory operations.

Design Decisions

Pivot for deterministic QuickSort: Last element, keeping implementation simple and consistent with textbook versions.

Pivot for randomized QuickSort: Random index swapped with the last element before partitioning.

Partitioning: Hoare partition was chosen for the randomized version to avoid degenerate behavior on duplicate-heavy arrays.

Data Structures: Simple static arrays were used to minimize overhead and isolate algorithmic performance.

Challenges Encountered

Lomuto partition produced misleadingly slow runtimes on certain datasets.

Ensuring consistent measurement required multiple runs due to natural runtime variability.

Managing recursion depth and avoiding stack overhead required careful handling of base cases.

4. Experimental Setup
4.1 Environment

Programming Language: Cpp

Compiler: GCC (default optimization level)

OS: Standard desktop environment

Timing Method: wall-clock measurement using clock()

4.2 Datasets

Four dataset types were used to capture a range of algorithm behaviors:

Random arrays

Sorted arrays

Reverse-sorted arrays

Duplicate-heavy arrays

Array sizes varied from small samples to large datasets, with each configuration tested multiple times to compute average runtime.

5. Results & Analysis
5.1 Overview

The results were presented using graphs and tables (already generated earlier). Key observations:

Deterministic QuickSort

Performs well on random data.

Shows severe slowdown on sorted and reverse-sorted arrays.

Often follows worst-case recursion patterns.

Randomized QuickSort (Hoare Partition)

Performance remains consistently close to O(n log n) across all input types.

Handles duplicates efficiently, unlike the Lomuto-based version.

Exhibits smoother scaling with array size.

5.2 Metrics Used

Wall-clock execution time (averaged over multiple runs)

Recursion depth (implicit through stack usage)

Operation count (approximated via comparisons and swaps)

5.3 Comparison Against Theory

Deterministic QuickSort deviates significantly from its average-case complexity on structured inputs.

Randomized QuickSort closely matches theoretical expectations thanks to good pivot distribution.

5.4 Why Runtime Varies Between Runs

Even deterministic QuickSort showed runtime differences across repeated executions on identical inputs. This is due to:

Cache hits/misses

TLB behavior

Branch prediction

CPU frequency scaling (Turbo Boost)

Pipeline stalls

OS thread scheduling

Background processes

Thread migration between cores

Memory alignment differences

These hardware-level factors introduce natural timing noise that cannot be eliminated in empirical studies.

6. Conclusion

This project demonstrates the practical importance of pivot selection and partitioning strategy in QuickSort implementations. Deterministic QuickSort performs well in favorable conditions but degrades significantly on already sorted or reverse-sorted data. Randomized QuickSort, especially when paired with Hoare partition, provides consistent and robust performance across a wide variety of input types.

The theoretical predictions align well with empirical measurements once partitioning is chosen appropriately. System-level factors introduce slight variations in runtime, but overall trends remain clear and interpretable. Randomized QuickSort is the more reliable choice for general-purpose sorting, which explains its use in many industrial libraries.

7. References

Cormen, Leiserson, Rivest, and Stein. Introduction to Algorithms.

Sedgewick and Wayne. Algorithms.

Lecture materials and standard algorithm texts.