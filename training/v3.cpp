#include <iostream>
#include <limits>
#include <ctime>
#include <chrono>
#include <vector>


using namespace std;

typedef float float8_t __attribute__((vector_size(8 * sizeof(float))));

static inline float hmin8(float8_t vv) {
    // TO DO : Return the min float of vv
}

static inline float8_t min8(float8_t x, float8_t y) {
    // TO DO : For each index return the min of x and y
    // For example x = {1,2,3,4,5,6,7,28} and y = {11,12,13,14,15,16,17,8} should return {1,2,3,4,5,6,7,8}
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