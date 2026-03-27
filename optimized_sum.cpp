#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

double optimized_sum(const double* a, int n) {
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < n; i += 2) {
        sum1 += a[i];
        sum2 += a[i + 1];
    }
    return sum1 + sum2;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n>" << endl;
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        cerr << "n must be positive" << endl;
        return 1;
    }

    
    int REPEAT;
    if (n == 64) REPEAT = 700000000;
    else if (n == 256) REPEAT = 150000000;
    else if (n == 1024) REPEAT = 80000000;
    else if (n == 4096) REPEAT = 30000000;
    else if (n == 16384) REPEAT =10000000;
    else if (n == 1048576) REPEAT = 30000;
    else REPEAT = 1000;

    double* a = new double[n];
    for (int i = 0; i < n; ++i) {
        a[i] = static_cast<double>(i);
    }

    volatile double dummy = optimized_sum(a, n);

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int r = 0; r < REPEAT; ++r) {
        dummy = optimized_sum(a, n);
    }
    QueryPerformanceCounter(&end);

    double total = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    double avg_ms = total / REPEAT * 1000.0;

    cout << fixed << setprecision(6);
    cout << "n=" << n << " optimized=" << avg_ms << " ms" << endl;

    delete[] a;
    return 0;
}
