#include "primes.hpp"


void primes_stdexec(size_t num_threads, size_t value) {
 
  std::vector<bool> prime(value, false);

  exec::static_thread_pool ctx{num_threads};

  auto sch = ctx.get_scheduler(); 

  auto computation
    = stdexec::schedule(sch)
    | stdexec::bulk(prime.size(), [&](size_t i){
        prime[i] = is_prime(i);
      })
    | stdexec::then([&](){
      return std::accumulate(prime.begin(), prime.end(), 0);
    });

  auto sum = stdexec::sync_wait(std::move(computation)).value();

}


std::chrono::microseconds measure_time_stdexec(size_t num_threads, size_t value) {
  auto beg = std::chrono::high_resolution_clock::now();
  primes_stdexec(num_threads, value);
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}


