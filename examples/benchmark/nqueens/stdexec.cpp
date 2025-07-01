#include "nqueens.hpp"

//using Solution = std::vector<int>;
//using SolutionSet = std::vector<Solution>;
//using sol_sender = exec::any_receiver_ref<
//    stdexec::completion_signatures<
//        stdexec::set_value_t(SolutionSet),
//        stdexec::set_error_t(std::exception_ptr),
//        stdexec::set_stopped_t()
//    >
//>::any_sender<>;


// Combine a vector of senders into one sender producing a merged SolutionSet
sol_sender join_all(std::vector<sol_sender>&& senders) {
  if (senders.empty()) {
    return stdexec::just(SolutionSet{});
  }

  if (senders.size() == 1) {
    return std::move(senders[0]);
  }

  sol_sender result = std::move(senders[0]);
  for (std::size_t i = 1; i < senders.size(); ++i) {
    result 
      = stdexec::when_all(std::move(result), std::move(senders[i]))
      | stdexec::then([](SolutionSet a, SolutionSet b) {
        a.insert(a.end(), b.begin(), b.end());
        return a;
      });
  }

  return result;
}

// Parallel recursive N-Queens solver
sol_sender nqueens(int n, int row, Solution partial, exec::static_thread_pool::scheduler sched) {

  if (row == n) {
    return stdexec::just(SolutionSet{partial});
  }

  std::vector<sol_sender> senders;

  for (int col = 0; col < n; ++col) {

    if (is_valid(partial, col)) {

      Solution next = partial;
      next.push_back(col);

      auto s 
        = stdexec::schedule(sched)
        | stdexec::let_value([=]() {
          return nqueens(n, row + 1, next, sched);
        });

      senders.push_back(std::move(s));
    }
  }

  return join_all(std::move(senders));
}


std::chrono::microseconds measure_time_stdexec(size_t num_threads, size_t num_nqueens) {

  auto beg = std::chrono::high_resolution_clock::now();

  exec::static_thread_pool pool{num_threads};

  auto sched = pool.get_scheduler();

  auto final_sender = nqueens(num_nqueens, 0, {}, sched);

  auto [result] = stdexec::sync_wait(std::move(final_sender)).value();

  auto end = std::chrono::high_resolution_clock::now();

  //assert(result == answers[num_queens]);

  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}
