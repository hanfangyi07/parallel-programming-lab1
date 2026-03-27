#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

double naive_sum(const double* a, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += a[i];
    }
    return sum;
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

    // 根据 n 动态调整重复次数，使总时间足够长
     int REPEAT;
    if (n == 64) REPEAT = 80000000;
    else if (n == 256) REPEAT = 30000000;
    else if (n == 1024) REPEAT = 2000000;
    else if (n == 4096) REPEAT = 500000;
    else if (n == 16384) REPEAT = 120000;
    else if (n == 1048576) REPEAT = 2000;
    else REPEAT = 1000;

    double* a = new double[n];
    for (int i = 0; i < n; ++i) {
        a[i] = static_cast<double>(i);
    }

    // 预热
    volatile double dummy = naive_sum(a, n);

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int r = 0; r < REPEAT; ++r) {
        dummy = naive_sum(a, n);
    }
    QueryPerformanceCounter(&end);

    double total = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    double avg_ms = total / REPEAT * 1000.0;

   
    cout << fixed << setprecision(6);
    cout << "n=" << n << " naive=" << avg_ms << " ms" << endl;

    delete[] a;
    return 0;
}
