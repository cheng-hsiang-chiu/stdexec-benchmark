#include "linear_chain.hpp"

void linear_chain_stdexec(size_t length, unsigned num_threads) {

  size_t counter = 0;

  exec::static_thread_pool ctx{num_threads};

  auto sch = ctx.get_scheduler();

  auto op = [&]() { ++counter; };

  using Sender = decltype(stdexec::schedule(sch) | stdexec::then(op));

  std::vector<std::variant<Sender>> senders;

  for (size_t i = 0; i < length; ++i) {
    senders.emplace_back(stdexec::schedule(sch) | stdexec::then(op));
  }

  // Run each sender
  for (auto& variant_sender : senders) {
    std::visit([](auto&& s) {
      stdexec::sync_wait(std::move(s));
    }, variant_sender);
  }
}


std::chrono::microseconds measure_time_stdexec(
  size_t length,
  unsigned num_threads ) {

  auto beg = std::chrono::high_resolution_clock::now();
  linear_chain_stdexec(length, num_threads);
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(end - beg);
}


