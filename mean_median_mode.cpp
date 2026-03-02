#include <iostream>
#include <cmath>

using namespace std;

// =====================================================
// UTILITIES (DRY)
// =====================================================
void mySwap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

struct ModeInfo {
    int max_freq;
    int mode_val;
};

// =====================================================
// PURE OBJECT-ORIENTED STRUCTURE
// =====================================================
class DescriptiveStatistics {
private:
    int* vals;
    int* freqs;
    int n;

    // =====================================================
    // Divide & Conquer Data Reading Phase
    // =====================================================
    void readSingleData(int index) {
        cout << "Enter Value and Frequency for item " << index + 1 << ": ";
        cin >> vals[index] >> freqs[index];
    }

    void readDataRecursive(int left, int right) {
        (left > right) ? void() :
        (left == right) ? readSingleData(left) :
        (readDataRecursive(left, (left + right) / 2), readDataRecursive((left + right) / 2 + 1, right));
    }

    // =====================================================
    // Divide & Conquer Sorting (Quick Sort) Phase
    // =====================================================
    int getPartitionIndex(int pivot, int right, int j, int i) {
        return (i >= right) ? j :
               (vals[i] < pivot) ? (mySwap(vals[i], vals[j]), mySwap(freqs[i], freqs[j]), getPartitionIndex(pivot, right, j + 1, i + 1)) :
               getPartitionIndex(pivot, right, j, i + 1);
    }

    int partitionElements(int left, int right) {
        return getPartitionIndex(vals[right], right, left, left);
    }

    void applyQuickSort(int left, int right, int pivotIdx) {
        (mySwap(vals[pivotIdx], vals[right]), mySwap(freqs[pivotIdx], freqs[right]), 0);
        quickSortRecursive(left, pivotIdx - 1);
        quickSortRecursive(pivotIdx + 1, right);
    }

    void quickSortRecursive(int left, int right) {
        (left < right) ? applyQuickSort(left, right, partitionElements(left, right)) : void();
    }

    // =====================================================
    // Divide & Conquer Sums Phase
    // =====================================================
    double sumXF(int left, int right) const {
        return (left == right) ? (double)vals[left] * freqs[left] :
               sumXF(left, (left + right) / 2) + sumXF((left + right) / 2 + 1, right);
    }

    int sumFreq(int left, int right) const {
        return (left == right) ? freqs[left] :
               sumFreq(left, (left + right) / 2) + sumFreq((left + right) / 2 + 1, right);
    }

    // =====================================================
    // Divide & Conquer Median Search Phase
    // =====================================================
    int evaluateMedianDandC(int left, int right, int target, int leftSum) const {
        return (leftSum >= target) ? 
               getMedianElementDandC(left, (left + right) / 2, target) :
               getMedianElementDandC((left + right) / 2 + 1, right, target - leftSum);
    }

    int getMedianElementDandC(int left, int right, int target) const {
        return (left == right) ? vals[left] :
               evaluateMedianDandC(left, right, target, sumFreq(left, (left + right) / 2));
    }

    double calculateMedianGivenTotal(int totalFreq) const {
        return (totalFreq % 2 != 0) ? 
               (double)getMedianElementDandC(0, n - 1, totalFreq / 2 + 1) :
               ((double)getMedianElementDandC(0, n - 1, totalFreq / 2) + 
                getMedianElementDandC(0, n - 1, totalFreq / 2 + 1)) / 2.0;
    }

    // =====================================================
    // Divide & Conquer Mode Search Phase
    // =====================================================
    ModeInfo combineMode(ModeInfo a, ModeInfo b) const {
        return (a.max_freq >= b.max_freq) ? a : b;
    }

    ModeInfo getModeInfo(int left, int right) const {
        return (left == right) ? ModeInfo{freqs[left], vals[left]} :
               combineMode(getModeInfo(left, (left + right) / 2),
                           getModeInfo((left + right) / 2 + 1, right));
    }

    // =====================================================
    // Divide & Conquer Measures of Dispersion Phase
    // =====================================================
    double sumSquaredDiff(double mean, int left, int right) const {
        return (left == right) ? (double)freqs[left] * (vals[left] - mean) * (vals[left] - mean) :
               sumSquaredDiff(mean, left, (left + right) / 2) + sumSquaredDiff(mean, (left + right) / 2 + 1, right);
    }

    double sumAbsDiff(double mean, int left, int right) const {
        return (left == right) ? (double)freqs[left] * std::abs(vals[left] - mean) :
               sumAbsDiff(mean, left, (left + right) / 2) + sumAbsDiff(mean, (left + right) / 2 + 1, right);
    }

public:
    // Constructor handles dynamic memory allocation
    DescriptiveStatistics(int size) : n(size) {
        vals = (n > 0) ? new int[n] : nullptr;
        freqs = (n > 0) ? new int[n] : nullptr;
    }

    // Destructor ensures memory cleanup
    ~DescriptiveStatistics() {
        delete[] vals;
        delete[] freqs;
    }

    bool isValid() const {
        return n > 0 && vals != nullptr && freqs != nullptr;
    }

    void inputAndSort() {
        (n > 0) ? (
            readDataRecursive(0, n - 1),
            quickSortRecursive(0, n - 1),
            void()
        ) : void();
    }

    double getMean() const {
        return sumXF(0, n - 1) / sumFreq(0, n - 1);
    }

    double getMedian() const {
        return calculateMedianGivenTotal(sumFreq(0, n - 1));
    }

    int getMode() const {
        return getModeInfo(0, n - 1).mode_val;
    }

    int getRange() const {
        return vals[n - 1] - vals[0];
    }

    double getVariance() const {
        return sumSquaredDiff(getMean(), 0, n - 1) / sumFreq(0, n - 1);
    }

    double getStdDev() const {
        return std::sqrt(getVariance());
    }

    double getMeanDev() const {
        return sumAbsDiff(getMean(), 0, n - 1) / sumFreq(0, n - 1);
    }

    void displayStatistics() const {
        (n > 0) ? (
            cout << "\n--- Statistics ---\n",
            cout << "Mean   : " << getMean() << "\n",
            cout << "Median : " << getMedian() << "\n",
            cout << "Mode   : " << getMode() << "\n",
            
            cout << "\n--- Measures of Dispersion ---\n",
            cout << "Range          : " << getRange() << "\n",
            cout << "Variance       : " << getVariance() << "\n",
            cout << "Std Deviation  : " << getStdDev() << "\n",
            cout << "Mean Deviation : " << getMeanDev() << "\n",
            
            void()
        ) : (cout << "No data available.\n", void());
    }
};

// =====================================================
// Main Execution
// =====================================================
int main() {
    int n;
    cout << "Welcome To PICT schools \nYou are experiencing a Calculator Programm which is Guided by a Math faculty and made by Technics given by CE faculty" << endl;
    cout << "Presenting the basic descriptive statistics calculator " << endl;
    cout << "Enter number of distinct data points: ";
    cin >> n;
    
    // Instantiating the encapsulated Object-Oriented component
    DescriptiveStatistics stats(n);
    
    // Using ternary sequencing in main, strictly adhering to the "absolutely zero if/else or loops" main methodology
    stats.isValid() ? (
        stats.inputAndSort(),
        stats.displayStatistics(),
        void()
    ) : (cout << "Invalid number of data points. Minimum 1 required.\n", void());
    
    return 0;
}
