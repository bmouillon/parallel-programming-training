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
    constexpr int nb = 8;
    int na = (n + nb - 1) / nb;

    constexpr int nd = 3;
    int nc = (n + nd - 1) / nd;
    int ncd = nc * nd;

    std::vector<float8_t> vd(ncd * na);
    std::vector<float8_t> vt(ncd * na);

#pragma omp parallel for
    for (int j = 0; j < n; ++j) {
        for (int ka = 0; ka < na; ++ka) {
            for (int kb = 0; kb < nb; ++kb) {
                int i = ka * nb + kb;
                vd[na * j + ka][kb] = i < n ? d[n * j + i] : infty;
                vt[na * j + ka][kb] = i < n ? d[n * i + j] : infty;
            }
        }
    }
    for (int j = n; j < ncd; ++j) {
        for (int ka = 0; ka < na; ++ka) {
            for (int kb = 0; kb < nb; ++kb) {
                vd[na * j + ka][kb] = infty;
                vt[na * j + ka][kb] = infty;
            }
        }
    }

#pragma omp parallel for
    for (int ic = 0; ic < nc; ++ic) {
        for (int jc = 0; jc < nc; ++jc) {
            float8_t vv[nd][nd];
            for (int id = 0; id < nd; ++id) {
                for (int jd = 0; jd < nd; ++jd) {
                    vv[id][jd] = f8infty;
                }
            }
            for (int ka = 0; ka < na; ++ka) {
                float8_t y0 = vt[na * (jc * nd + 0) + ka];
                float8_t y1 = vt[na * (jc * nd + 1) + ka];
                float8_t y2 = vt[na * (jc * nd + 2) + ka];
                float8_t x0 = vd[na * (ic * nd + 0) + ka];
                float8_t x1 = vd[na * (ic * nd + 1) + ka];
                float8_t x2 = vd[na * (ic * nd + 2) + ka];
                vv[0][0] = min8(vv[0][0], x0 + y0);
                vv[0][1] = min8(vv[0][1], x0 + y1);
                vv[0][2] = min8(vv[0][2], x0 + y2);
                vv[1][0] = min8(vv[1][0], x1 + y0);
                vv[1][1] = min8(vv[1][1], x1 + y1);
                vv[1][2] = min8(vv[1][2], x1 + y2);
                vv[2][0] = min8(vv[2][0], x2 + y0);
                vv[2][1] = min8(vv[2][1], x2 + y1);
                vv[2][2] = min8(vv[2][2], x2 + y2);
            }
            for (int id = 0; id < nd; ++id) {
                for (int jd = 0; jd < nd; ++jd) {
                    int i = ic * nd + id;
                    int j = jc * nd + jd;
                    if (i < n && j < n) {
                        r[n * i + j] = hmin8(vv[id][jd]);
                    }
                }
            }
        }
    }
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