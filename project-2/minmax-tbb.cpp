

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <tbb/tbb.h>

constexpr size_t N = 100000000;

using namespace std;

double rng() {
  thread_local static std::default_random_engine gen;
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  return dist(gen);
}

int main() {
  vector<double> v(N);

  // Initialize vector
  std::generate(v.begin(), v.end(), rng);

  auto start = chrono::high_resolution_clock::now();

  // Find minimum and maximum elements using TBB parallel loop
  tbb::combinable<std::pair<double, double>> minmax_combinable([&]() {
    return make_pair(v[0], v[0]);
  });

  tbb::parallel_for(tbb::blocked_range<vector<double>::iterator>(v.begin() + 1, v.end()),
    [&](const tbb::blocked_range<vector<double>::iterator>& range) {
      auto& local_minmax = minmax_combinable.local();
      for (auto p = range.begin(); p != range.end(); ++p) {
        local_minmax.first = std::min(local_minmax.first, *p);
        local_minmax.second = std::max(local_minmax.second, *p);
      }
    }
  );
  
  auto minmax = minmax_combinable.combine([](const auto& a, const auto& b) {
    return make_pair(std::min(a.first, b.first), std::max(a.second, b.second));
  });

  auto stop = chrono::high_resolution_clock::now();

  // Check results
  const auto [minp, maxp] = std::minmax_element(v.begin(), v.end());

  if ((minmax.first != *minp) || (minmax.second != *maxp)) {
    cout << "error" << endl;
  }

  // Free memory
  v.clear();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  cout << 1e-6 * duration.count() << " sec" << endl;

  return 0;
}
