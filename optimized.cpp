#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

void optimized(const double* A, const double* x, double* y, int n) {
   
    for (int i = 0; i < n; ++i) {
        y[i] = 0.0;
    }
    // 逐行遍历矩阵
    for (int i = 0; i < n; ++i) {
        double xi = x[i];               // 当前行的向量元素
        for (int j = 0; j < n; ++j) {
            y[j] += A[i * n + j] * xi; // 连续访问第 i 行的所有列
        }
    }
}

int main(int argc, char* argv[]) {
    // 检查命令行参数
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n>" << endl;
        return 1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        cerr << "n must be positive" << endl;
        return 1;
    }

    // 动态调整重复次数，保证每种规模运行时间足够长（便于 profiling）
    int REPEAT = 1000;
    if (n <= 64) REPEAT = 5000000;
    else if (n <= 128) REPEAT = 3000000;
    else if (n <= 256) REPEAT = 100000;
    else if (n <= 512) REPEAT = 50000;
    else REPEAT = 10000;

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
    optimized(A, x, y, n);

    // 计时
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int r = 0; r < REPEAT; ++r) {
        optimized(A, x, y, n);
    }
    QueryPerformanceCounter(&end);
    double total = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    double avg_ms = total / REPEAT * 1000.0;

    cout << fixed << setprecision(3);
    cout << "n = " << n << ", optimized time = " << avg_ms << " ms" << endl;

    delete[] A;
    delete[] x;
    delete[] y;
    return 0;
}
