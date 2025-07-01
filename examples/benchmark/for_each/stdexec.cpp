#include "for_each.hpp"


void for_each_stdexec(size_t num_threads) {

  exec::static_thread_pool ctx{num_threads};

  auto sch = ctx.get_scheduler(); 

  auto computation
    = stdexec::schedule(sch)
    | stdexec::bulk(vec.size(), [&](size_t i){
        vec[i] = std::tan(vec[i]);
      });

  stdexec::sync_wait(std::move(computation));
}


std::chrono::microseconds measure_time_stdexec(size_t num_threads) {
  
  auto beg = std::chrono::high_resolution_clock::now();
  for_each_stdexec(num_threads);
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}


