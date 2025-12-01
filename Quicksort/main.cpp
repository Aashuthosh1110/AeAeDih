#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

// ---------------------- DETERMINISTIC (LAST PIVOT) -----------------------

int partitionDet(vector<int> &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksortDet(vector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partitionDet(arr, low, high);
        quicksortDet(arr, low, pi - 1);
        quicksortDet(arr, pi + 1, high);
    }
}

// ---------------------- RANDOMIZED QUICKSORT (HOARE) -----------------------

int randomizedHoarePartition(vector<int>& arr, int low, int high) {
    int pivotIndex = low + rand() % (high - low + 1);
    int pivot = arr[pivotIndex];

    int i = low - 1;
    int j = high + 1;

    while (true) {
        do { i++; } while (arr[i] < pivot);
        do { j--; } while (arr[j] > pivot);

        if (i >= j)
            return j;

        swap(arr[i], arr[j]);
    }
}

void randomizedQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int p = randomizedHoarePartition(arr, low, high);
        randomizedQuickSort(arr, low, p);
        randomizedQuickSort(arr, p + 1, high);
    }
}

// ---------------------- CSV LOADER -----------------------

vector<int> loadCSV(string filename) {
    vector<int> arr;
    ifstream file(filename);
    string line, val;

    if (getline(file, line)) {
        stringstream ss(line);
        while (getline(ss, val, ',')) {
            if(!val.empty())
                arr.push_back(stoi(val));
        }
    }
    return arr;
}

// ---------------------- BENCHMARK FUNCTION -----------------------

struct Stats {
    double best = 1e18;
    double worst = 0;
    double total = 0;
};

Stats benchmark(function<void(vector<int>&)> sorter, vector<int> arr, int runs) {
    Stats s;

    for (int i = 0; i < runs; i++) {
        vector<int> copy = arr;

        auto start = chrono::high_resolution_clock::now();
        sorter(copy);
        auto end = chrono::high_resolution_clock::now();

        double ms = chrono::duration<double, milli>(end - start).count();
        s.best = min(s.best, ms);
        s.worst = max(s.worst, ms);
        s.total += ms;
    }
    return s;
}

// ---------------------- MAIN PROGRAM -----------------------

int main() {
    srand(time(NULL));

    string folder = "qsort_datasets/";
    vector<string> files;

    for (auto &entry : filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".csv") {
            files.push_back(entry.path().string());
        }
    }

    int runs = 100;

    // Create CSV output file
    ofstream out("runtime_results.csv");
    out << "dataset_name,det_best,det_avg,det_worst,rand_best,rand_avg,rand_worst\n";

    cout << "Running " << runs << " iterations on each dataset...\n\n";

    for (string file : files) {

        vector<int> arr = loadCSV(file);
        string dataset = filesystem::path(file).filename().string();

        cout << "------ " << dataset << " (size = " << arr.size() << ") ------\n";

        // Deterministic QS
        Stats det = benchmark(
            [&](vector<int> &a) { quicksortDet(a, 0, a.size() - 1); },
            arr, runs
        );

        // Randomized QS
        Stats rnd = benchmark(
            [&](vector<int> &a) { randomizedQuickSort(a, 0, a.size() - 1); },
            arr, runs
        );

        double det_avg = det.total / runs;
        double rnd_avg = rnd.total / runs;

        // Print to console
        cout << "Deterministic Quicksort -> Best: " << det.best 
             << "  Worst: " << det.worst 
             << "  Avg: " << det_avg << "\n";

        cout << "Randomized Quicksort    -> Best: " << rnd.best 
             << "  Worst: " << rnd.worst 
             << "  Avg: " << rnd_avg << "\n";

        cout << "-------------------------------------------------------\n\n";

        // Write to CSV
        out << dataset << ","
            << det.best << "," << det_avg << "," << det.worst << ","
            << rnd.best << "," << rnd_avg << "," << rnd.worst << "\n";
    }

    out.close();

    cout << "Results saved to runtime_results.csv\n";

    // Run Python plotting script
    cout << "Generating graphs using plot_qsort.py...\n";
    system("python plot_qsort.py");

    cout << "Done.\n";
    return 0;
}
