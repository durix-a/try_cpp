#include <iostream>
#include <algorithm>
#include <ctime>
#include <thread>
#include "MatrixUtils.h"

using namespace std;

void MatrixOperations();
void ValidityTest();
void PerfTest();

int main(int argc, char* argv[]) {
    MatrixOperations();
}

void MatrixOperations() {
    // PerfTest();
    ValidityTest();
}

void PerfTest() {
    clock_t c_start;
    clock_t c_end;
    long double time_elapsed_ms;
    // Matrix d = MatrixUtils::random(100, 300);
    // Matrix e = MatrixUtils::random(300, 100);
    // Matrix g = MatrixUtils::random(100, 300);
    // Matrix h = MatrixUtils::random(300, 100);
    // Matrix d = MatrixUtils::random(1000, 3000);
    // Matrix e = MatrixUtils::random(3000, 1000);
    // Matrix g = MatrixUtils::random(1000, 3000);
    // Matrix h = MatrixUtils::random(3000, 1000);
    Matrix d = MatrixUtils::random(500, 2000);
    Matrix e = MatrixUtils::random(2000, 500);
    Matrix g = MatrixUtils::random(500, 2000);
    Matrix h = MatrixUtils::random(2000, 500);

    MatrixMultiplier mm1(d, e);
    MatrixMultiplier mm2(g, h);

    // c_start = clock();
    // thread th1(ref(mm1));
    // thread th2(ref(mm2));
    // th1.join();
    // th2.join();
    // c_end = clock();
    // time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    // cout << "CPU time used(joined): " << time_elapsed_ms << " ms\n";
    // cout << "CPU time used(mm1): " << mm1.getTimeElapsedMs() << " ms\n";
    // cout << "CPU time used(mm2): " << mm2.getTimeElapsedMs() << " ms\n";

    c_start = clock();
    mm1();
    mm2();
    c_end = clock();
    time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    cout << "CPU time used(2): " << time_elapsed_ms << " ms\n";
}

void ValidityTest() {
    int a_values[] = { 1, 2, 3, 4, 5, 6 };
    Matrix a(2, 3);
    int b_values[] = { 1, 2, 1, 2, 1, 2 };
    Matrix b(3, 2);
    Matrix d = MatrixUtils::random(4, 4);
    Matrix e = MatrixUtils::random(4, 4);

    copy_n(a_values, sizeof(a_values) / sizeof(int), a.getValues());
    copy_n(b_values, sizeof(b_values) / sizeof(int), b.getValues());

    // Matrix t = b;
    // t.transpose();

    // MatrixUtils::PrintMatrix(a);
    // cout << "-----------------------" << endl;
    // MatrixUtils::PrintMatrix(b);
    // cout << "-----------------------" << endl;
    // MatrixUtils::PrintMatrix(t);
    // cout << "-----------------------" << endl;
    MatrixUtils::PrintMatrix(d);
    cout << "-----------------------" << endl;
    MatrixUtils::PrintMatrix(e);
    cout << "-----------------------" << endl;

    Matrix c;
    Matrix f;
    // c = MatrixUtils::multiplyParalel(d, e, 1);
    // f = MatrixUtils::multiply(d, e);
    // cout << "d * e: " << (c == f ? "c == f" : "c != f") << endl;
    
    // c = MatrixUtils::multiplyParalel(e, d, 1);
    f = MatrixUtils::multiply(e, d);
    // cout  << "e * d: " << (c == f ? "c == f" : "c != f") << endl;
    
    c = MatrixUtils::multiplyParalel(e, d, 2);
    cout  << "e * d x2: " << (c == f ? "c == f" : "c != f") << endl;

    // c = MatrixUtils::multiplyParalel(a, b, 1);
    // f = MatrixUtils::multiply(a, b);
    // cout  << "a * b: " << (c == f ? "c == f" : "c != f") << endl;
    
    // c = MatrixUtils::multiplyParalel(b, a, 1);
    // f = MatrixUtils::multiply(b, a);
    // cout  << "b * a: " << (c == f ? "c == f" : "c != f") << endl;
    MatrixUtils::PrintMatrix(c);
    MatrixUtils::PrintMatrix(f);
}
