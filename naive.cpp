#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

void naive(const double* A, const double* x, double* y, int n) {
    for (int j = 0; j < n; ++j) {
        y[j] = 0.0;
        for (int i = 0; i < n; ++i) {
            y[j] += A[i * n + j] * x[i];
        }
    }
}

int main(int argc, char* argv[]) {
    // 检查命令行参数，第一个参数是 n
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n>" << endl;
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        cerr << "n must be positive" << endl;
        return 1;
    }

   int REPEAT = 1000; // 基础值
  if (n <=128) REPEAT = 10000;   // 小规模大幅增加
   else if (n <= 256) REPEAT = 8000;
   
    else REPEAT = 2000;

    double* A = new double[n * n];
    double* x = new double[n];
    double* y = new double[n];

    // 初始化数据
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i * n + j] = static_cast<double>(i + j);
        }
        x[i] = static_cast<double>(i);
    }

    // 预热
    naive(A, x, y, n);

    // 计时部分
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int r = 0; r < REPEAT; ++r) {
        naive(A, x, y, n);
    }
    QueryPerformanceCounter(&end);
    double total = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    double avg_ms = total / REPEAT * 1000.0;

    cout << fixed << setprecision(3);
    cout << "n = " << n << ", naive time = " << avg_ms << " ms" << endl;

    delete[] A;
    delete[] x;
    delete[] y;
    return 0;
}
