#include <iostream>

using namespace std;

// =====================================================
// UTILITIES (DRY)
// =====================================================
void mySwap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

// =====================================================
// Divide & Conquer Data Reading Phase
// =====================================================
void readSingleData(int* vals, int* freqs, int index) {
    cout << "Enter Value and Frequency for item " << index + 1 << ": ";
    cin >> vals[index] >> freqs[index];
}

void readData(int* vals, int* freqs, int left, int right) {
    (left > right) ? void() :
    (left == right) ? readSingleData(vals, freqs, left) :
    (readData(vals, freqs, left, (left + right) / 2), readData(vals, freqs, (left + right) / 2 + 1, right));
}

// =====================================================
// Divide & Conquer Sorting (Quick Sort) Phase
// We need sorting primarily for calculating the Median
// =====================================================
void quickSort(int* vals, int* freqs, int left, int right);

int getPartitionIndex(int *vals, int *freqs, int pivot, int right, int j, int i) {
    return (i >= right) ? j :
           (vals[i] < pivot) ? (mySwap(vals[i], vals[j]), mySwap(freqs[i], freqs[j]), getPartitionIndex(vals, freqs, pivot, right, j + 1, i + 1)) :
           getPartitionIndex(vals, freqs, pivot, right, j, i + 1);
}

int partitionElements(int *vals, int *freqs, int left, int right) {
    return getPartitionIndex(vals, freqs, vals[right], right, left, left);
}

void applyQuickSort(int* vals, int* freqs, int left, int right, int pivotIdx) {
    (mySwap(vals[pivotIdx], vals[right]), mySwap(freqs[pivotIdx], freqs[right]), 0);
    quickSort(vals, freqs, left, pivotIdx - 1);
    quickSort(vals, freqs, pivotIdx + 1, right);
}

void quickSort(int* vals, int* freqs, int left, int right) {
    (left < right) ? applyQuickSort(vals, freqs, left, right, partitionElements(vals, freqs, left, right)) : void();
}

// =====================================================
// Divide & Conquer Sums (Used for Mean) Phase
// =====================================================
double sumXF(int* vals, int* freqs, int left, int right) {
    return (left == right) ? (double)vals[left] * freqs[left] :
           sumXF(vals, freqs, left, (left + right) / 2) + sumXF(vals, freqs, (left + right) / 2 + 1, right);
}

int sumFreq(int* freqs, int left, int right) {
    return (left == right) ? freqs[left] :
           sumFreq(freqs, left, (left + right) / 2) + sumFreq(freqs, (left + right) / 2 + 1, right);
}

// -----------------------------------------------------
// Mean Calculation
// -----------------------------------------------------
double getMean(int* vals, int* freqs, int n) {
    return sumXF(vals, freqs, 0, n - 1) / sumFreq(freqs, 0, n - 1);
}

// =====================================================
// Divide & Conquer Median Search Phase
// =====================================================
int getMedianElementDandC(int* vals, int* freqs, int left, int right, int target);

int evaluateMedianDandC(int* vals, int* freqs, int left, int right, int target, int leftSum) {
    return (leftSum >= target) ? 
           getMedianElementDandC(vals, freqs, left, (left + right) / 2, target) :
           getMedianElementDandC(vals, freqs, (left + right) / 2 + 1, right, target - leftSum);
}

// Searches for target frequency recursively in split arrays!
int getMedianElementDandC(int* vals, int* freqs, int left, int right, int target) {
    return (left == right) ? vals[left] :
           evaluateMedianDandC(vals, freqs, left, right, target, sumFreq(freqs, left, (left + right) / 2));
}

double calculateMedianGivenTotal(int* vals, int* freqs, int n, int totalFreq) {
    return (totalFreq % 2 != 0) ? 
           (double)getMedianElementDandC(vals, freqs, 0, n - 1, totalFreq / 2 + 1) :
           ((double)getMedianElementDandC(vals, freqs, 0, n - 1, totalFreq / 2) + 
            getMedianElementDandC(vals, freqs, 0, n - 1, totalFreq / 2 + 1)) / 2.0;
}

// -----------------------------------------------------
// Median Calculation
// -----------------------------------------------------
double getMedian(int* vals, int* freqs, int n) {
    return calculateMedianGivenTotal(vals, freqs, n, sumFreq(freqs, 0, n - 1));
}

// =====================================================
// Divide & Conquer Mode Search Phase
// =====================================================
struct ModeInfo {
    int max_freq;
    int mode_val;
};

ModeInfo combineMode(ModeInfo a, ModeInfo b) {
    // DRY comparison logic
    return (a.max_freq >= b.max_freq) ? a : b;
}

ModeInfo getModeInfo(int* vals, int* freqs, int left, int right) {
    return (left == right) ? ModeInfo{freqs[left], vals[left]} :
           combineMode(getModeInfo(vals, freqs, left, (left + right) / 2),
                       getModeInfo(vals, freqs, (left + right) / 2 + 1, right));
}

// -----------------------------------------------------
// Mode Calculation
// -----------------------------------------------------
int getMode(int* vals, int* freqs, int n) {
    return getModeInfo(vals, freqs, 0, n - 1).mode_val;
}

// =====================================================
// Main Execution
// =====================================================
int main() {
    int n;
    cout<<"Welcome To PICT schools \nYou are experiencing a Calculator Programm which is Guided by a Math faculty and made by Technics given by CE faculty"<<endl;
    cout<<"Presenting the basic descriptive statistics calculator "<<endl;
    cout << "Enter number of distinct data points: ";
    cin >> n;
    
    int* vals = new int[n];
    int* freqs = new int[n];
    
    // Absolutely zero if/else or loops in main, using ternary operator sequencing!
    (n > 0) ? (
        readData(vals, freqs, 0, n - 1),
        quickSort(vals, freqs, 0, n - 1),
        
        cout << "\n--- Statistics ---\n",
        cout << "Mean   : " << getMean(vals, freqs, n) << "\n",
        cout << "Median : " << getMedian(vals, freqs, n) << "\n",
        cout << "Mode   : " << getMode(vals, freqs, n) << "\n",
        
        void()
    ) : (cout << "Invalid number of data points. Minimum 1 required.\n", void());
    
    delete[] vals;
    delete[] freqs;
    
    return 0;
}
