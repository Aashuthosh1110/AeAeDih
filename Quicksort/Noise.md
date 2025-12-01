Even though deterministic quicksort do not have anything variable in its implementation, the runtime still varies when we run the algorithm on an input for 1000 times, so the probable cause for it are listed below:

1. CPU Cache Behavior
Data might be in L1/L2/L3 cache in some runs but not in others.
Cache hits are very fast, cache misses are much slower.
This creates noticeable variation in memory-access time.

2. TLB (Translation Lookaside Buffer) Misses
Memory address translations may or may not be cached.
TLB misses increase memory access latency, affecting runtime.

3. Branch Prediction Variability
Quicksort repeatedly uses conditional checks.
If the CPU predicts a branch correctly, execution is fast.
Mis-predictions flush the pipeline, causing sudden slowdowns.

4. Dynamic CPU Frequency Scaling
Technologies like Intel Turbo Boost or AMD Precision Boost change CPU frequency on the fly.
Processor may run faster when cool and slower when hot.
This causes measurable differences in runtime between runs.

5. Instruction Pipeline Stalls
Small differences in instruction order and prior CPU activity affect the pipeline.
Stalls or hazards change execution time even for identical code.

6. Operating System Scheduling
The OS may interrupt your program to run background tasks.
Context switches introduce timing noise.
Multiple runs experience different interruption patterns.

7. Background Processes
Applications like browsers, antivirus, cloud-sync, and system processes run concurrently.
Shared CPU time affects your program’s performance.

8. Thread Migration Across CPU Cores
OS may move your program from one core to another.
Different cores may have different cache states.
Causes variation in performance.

9. Stack and Heap Memory Layout
Memory alignment differs between runs.
This impacts cache line utilization and performance.

10. Other I/O and System Interrupts
Disk activity, network requests, and hardware interrupts slow the program unpredictably.
These occur in patterns that are not repeatable across runs.