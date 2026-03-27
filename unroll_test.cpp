#include <iostream>
#include <iomanip>
#include <windows.h>
#include <vector>

using namespace std;

// 2路链式累加
double optimized_sum_2way(const double* a, int n) {
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < n; i += 2) {
        sum1 += a[i];
        sum2 += a[i + 1];
    }
    return sum1 + sum2;
}

// 4路链式累加
double optimized_sum_4way(const double* a, int n) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    for (int i = 0; i < n; i += 4) {
        sum1 += a[i];
        sum2 += a[i + 1];
        sum3 += a[i + 2];
        sum4 += a[i + 3];
    }
    return sum1 + sum2 + sum3 + sum4;
}

// 8路链式累加 
double optimized_sum_8way(const double* a, int n) {
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    double sum5 = 0.0, sum6 = 0.0, sum7 = 0.0, sum8 = 0.0;
    for (int i = 0; i < n; i += 8) {
        sum1 += a[i];
        sum2 += a[i + 1];
        sum3 += a[i + 2];
        sum4 += a[i + 3];
        sum5 += a[i + 4];
        sum6 += a[i + 5];
        sum7 += a[i + 6];
        sum8 += a[i + 7];
    }
    return sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
}

int main() {
    // 测试规模
    vector<int> sizes = {256, 1024, 4096, 16384, 1048576};

    // 路数版本（名称和函数指针）
    vector<pair<string, double(*)(const double*, int)>> versions = {
        {"2way", optimized_sum_2way},
        {"4way", optimized_sum_4way},
        {"8way", optimized_sum_8way}
    };

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    cout << fixed << setprecision(6);

    // 对每个规模进行测试
    for (int n : sizes) {
        // 分配数组并初始化
        double* a = new double[n];
        for (int i = 0; i < n; ++i) a[i] = static_cast<double>(i);

        // 根据规模设定重复次数
        int repeat = 1000;
        if (n <= 256) repeat = 100000;
        else if (n <= 1024) repeat = 20000;
        else if (n <= 4096) repeat = 5000;
        else if (n <= 16384) repeat = 1200;
        else repeat = 20;   // 1048576

        cout << "n = " << n << " (repeat = " << repeat << ")" << endl;

        // 测试每个版本
        for (auto& ver : versions) {
            const string& name = ver.first;
            auto func = ver.second;

            // 预热
            volatile double dummy = func(a, n);

            // 正式计时
            QueryPerformanceCounter(&start);
            for (int r = 0; r < repeat; ++r) {
                dummy = func(a, n);
            }
            QueryPerformanceCounter(&end);

            double total = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
            double avg_ms = total / repeat * 1000.0;

            cout << "  " << name << ": " << avg_ms << " ms" << endl;
        }

        delete[] a;
        cout << endl;
    }

    return 0;
}
