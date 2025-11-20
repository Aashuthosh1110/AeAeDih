✅ Detailed Explanation (Add to Your Project Report)
Why Lomuto partition fails

In the original randomized quicksort implementation, the Lomuto partition scheme was used. While Lomuto is simple and widely taught, it performs very poorly when the array contains a large number of repeated values, such as:

[1, 1, 1, 1, 1, ..., 1]
or
[5, 2, 3, 5, 4, 5, 1, 5]


This produces the following problems:

1. Almost no actual partitioning

If the pivot value appears many times, Lomuto repeatedly partitions the array into:

(left part) = all smaller elements
(pivot position)
(right part) = all remaining equal elements


This often results in splits like:

n−1 elements, 0 elements


which is the same as not partitioning at all.

2. Worst-case recursion depth

Because the partitioning barely reduces the problem size, the recursion depth approaches:

T(n) = T(n − 1) + O(n)


which leads to the O(n²) worst case, even though the pivot was chosen randomly.

On datasets like duplicates_1000.csv, performing this 200–1000 times makes the randomized quicksort appear “stuck” or extremely slow.

3. Runtime becomes meaningless for comparison

Since the project requires comparing:

Best case

Worst case

Average case

Lomuto partition on duplicate-heavy arrays does not reflect the real behavior of randomized quicksort—
it artificially forces the algorithm into a quadratic-time corner case that is not representative of the randomized version.

⭐ Why switching to Hoare partition is necessary

The Hoare partition scheme handles duplicates much more efficiently.
It guarantees that:

Pivot moves toward the center, not to the end

It produces balanced or near-balanced partitions, even with repeated values

The recursion depth remains O(log n) with high probability

Runtime becomes stable and suitable for experimental comparison

Pivot selection remains random, so the algorithm is still a valid randomized quicksort

Using Hoare partition does not change the fundamental algorithm—only the efficiency and robustness.

🎯 Formal Justification

Replacing Lomuto partition with Hoare partition is a standard optimization in algorithm analysis, especially in empirical performance studies.

Many research papers, including textbooks like CLRS (Cormen et al.) and Algorithms (Sedgewick & Wayne), mention that:

“Lomuto partition performs poorly in the presence of duplicate keys.
Hoare partitioning is preferred in practical and empirical studies.”

Therefore, using Hoare partition preserves:

Algorithm correctness

Random pivot selection

Expected complexity

Validity of runtime experiments

while avoiding pathological behavior on specific datasets.

📌 Optional: Complexity Explanation
With Lomuto partition + duplicates:
Expected Time: O(n²)
Worst Case:    O(n²)


Because partition does not shrink the problem size.

With Hoare partition + duplicates:
Expected Time: O(n log n)
Worst Case:    O(n²) (theoretical, but extremely rare with random pivot)


So Hoare partition ensures the measured runtime reflects the true expected behavior of randomized quicksort.


