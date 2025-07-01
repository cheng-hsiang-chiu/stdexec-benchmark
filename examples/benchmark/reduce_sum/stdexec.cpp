#include "reduce_sum.hpp"

void reduce_sum_stdexec(unsigned num_threads) {
  
  exec::static_thread_pool pool{num_threads};

  std::vector<int> partial_sums(64, 0);
  size_t N = vec.size();

  auto s
    = stdexec::starts_on(pool.get_scheduler(),
      stdexec::just()
    | stdexec::bulk(64, [&](size_t chunk_index) {
            
      size_t chunk_size = (N + 63) / 64;
      size_t begin = chunk_index * chunk_size;
      size_t end = std::min(begin + chunk_size, N);

      for (std::size_t i = begin; i < end; ++i) {
        partial_sums[chunk_index] += vec[i];
      }
    })
    | stdexec::then([&]() {
      return std::accumulate(partial_sums.begin(), partial_sums.end(), 0);
    })
  );

  auto result = stdexec::sync_wait(std::move(s));
}


std::chrono::microseconds measure_time_stdexec(unsigned num_threads) {

  auto beg = std::chrono::high_resolution_clock::now();
  reduce_sum_stdexec(num_threads);
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}


