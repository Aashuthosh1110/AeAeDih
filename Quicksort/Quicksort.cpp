#include <bits/stdc++.h>
using namespace std;

int partitionDet(vector<int> &arr, int low, int high) {
    // choose middle element as pivot
    int mid = low + (high - low) / 2;
    int pivot = arr[mid];

    int i = low;
    int j = high;

    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    return i;
}

void quicksortDet(vector<int> &arr, int low, int high) {
    if (low < high) {
        int p = partitionDet(arr, low, high);

        // Notice: new boundaries for Hoare-style partition
        quicksortDet(arr, low, p - 1);
        quicksortDet(arr, p, high);
    }
}


int main() {
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    quicksortDet(arr, 0, arr.size() - 1);

    cout << "Sorted array: ";
    for(int x : arr) cout << x << " ";
}
