#include <iostream>
#include <limits>
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;

typedef float float8_t __attribute__((vector_size(8 * sizeof(float))));

constexpr float infty = std::numeric_limits<float>::infinity();

constexpr float8_t f8infty{
    infty, infty, infty, infty,
    infty, infty, infty, infty
};

static inline float hmin8(float8_t vv) {
    float v = infty;
    for (int i = 0; i < 8; ++i) {
        v = std::min(vv[i], v);
    }
    return v;
}

static inline float8_t min8(float8_t x, float8_t y) {
    return x < y ? x : y;
}

void step(float* r, const float* d, int n) {
    // TO DO
}

int main() {
    constexpr int n = 4000;
    // Generate a random graph
    vector<float> d(n * n);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < n * n; ++i) {
        d[i] = static_cast<float>(rand()) / RAND_MAX;
    }
    // Compute shortest 2-edge paths
    vector<float> r(n * n);
    auto start = chrono::high_resolution_clock::now();
    step(r.data(), d.data(), n);
    auto end = chrono::high_resolution_clock::now();
    // Display results
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << r[i * n + j] << " ";
        }
        cout << "\n";
    }
    chrono::duration<float> duration = end - start;
    cout << "Time elapsed: " << duration.count() << " seconds\n";
    return 0;
}