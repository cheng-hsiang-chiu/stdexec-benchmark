#include "sort.hpp"



template <typename T>
using sort_sender = typename exec::any_receiver_ref<
  stdexec::completion_signatures<
    stdexec::set_value_t(std::vector<T>),
    stdexec::set_error_t(std::exception_ptr),
    stdexec::set_stopped_t()
  >
>::template any_sender<>;


template <typename T>
sort_sender<T> sort_stdexec(std::vector<T> data) {
  if (data.size() <= 1) {
    return stdexec::just(std::move(data));
  }

  // Partition
  T pivot = data[0];
  std::vector<T> less, greater;
  for (size_t i = 1; i < data.size(); ++i) {
    (data[i] < pivot ? less : greater).push_back(data[i]);
  }

  // Recursive async sort of sub-parts
  auto sorted_less = sort_stdexec(std::move(less));
  auto sorted_greater = sort_stdexec(std::move(greater));

  return sort_sender<T>{stdexec::let_value(
    stdexec::when_all(std::move(sorted_less), std::move(sorted_greater)),
    [pivot](std::vector<T> a, std::vector<T> b) -> sort_sender<T> {
      a.push_back(pivot);
      a.insert(a.end(), b.begin(), b.end());

      // Must explicitly wrap just(...) into sort_sender<T>
      return sort_sender<T>{stdexec::just(std::move(a))};
    }
  )};
}




std::chrono::microseconds measure_time_stdexec(size_t num_threads) {

  auto beg = std::chrono::high_resolution_clock::now();
  exec::static_thread_pool pool(num_threads);
  auto s = sort_stdexec(vec);
  auto sort = stdexec::starts_on(pool.get_scheduler(), std::move(s));
  auto result = stdexec::sync_wait(std::move(sort));
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}


