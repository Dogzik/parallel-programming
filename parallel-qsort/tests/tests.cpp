#include <chrono>
#include <random>
#include <iostream>
#include <cilk/cilk_api.h>

#include "src/parallel/map.h"
#include "src/sequential/map.h"
#include "src/sequential/filter.h"

#include "src/sequential/scan.h"
#include "src/parallel/scan.h"

int main() {
  std::default_random_engine generator(time(nullptr));
  raw_array<int> a(100'000'000);
  for (int i = 0; i < a.size(); ++i) {
    a.emplace_at(i, i + 1);
  }
  std::chrono::steady_clock::time_point begin_par = std::chrono::steady_clock::now();
  auto res_par = seq::inclusive_scan(a);
  std::chrono::steady_clock::time_point end_par  = std::chrono::steady_clock::now();
  std::cout << "par::map took " << std::chrono::duration_cast<std::chrono::microseconds>(end_par - begin_par).count() << " microseconds\n";

  std::chrono::steady_clock::time_point begin_seq = std::chrono::steady_clock::now();
  auto res_seq = seq::exclusive_scan(a).first;
  std::chrono::steady_clock::time_point end_seq  = std::chrono::steady_clock::now();
  std::cout << "seq::map took " << std::chrono::duration_cast<std::chrono::microseconds>(end_seq - begin_seq).count() << " microseconds\n";
  std::cout << res_par[generator() % a.size()] << "\n" << res_seq[generator() % a.size()] << std::endl;
  return 0;
}
