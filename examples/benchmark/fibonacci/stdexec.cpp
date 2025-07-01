#include "fibonacci.hpp"

auto serial_fib(long n) -> long {
  return n < 2 ? n : serial_fib(n - 1) + serial_fib(n - 2);
}

template <class... Ts>
using any_sender_of =
  typename exec::any_receiver_ref<stdexec::completion_signatures<Ts...>>::template any_sender<>;

using fib_sender = any_sender_of<stdexec::set_value_t(long)>;

template <typename Scheduler>
struct fib_s {
  using sender_concept = stdexec::sender_t;
  using completion_signatures = stdexec::completion_signatures<stdexec::set_value_t(long)>;

  long cutoff;
  long n;
  Scheduler sched;

  template <class Receiver>
  struct operation {
    Receiver rcvr_;
    long cutoff;
    long n;
    Scheduler sched;

    void start() & noexcept {
      if (n < cutoff) {
        stdexec::set_value(static_cast<Receiver&&>(rcvr_), serial_fib(n));
      } else {
        auto mkchild = [&](long n) {
          return stdexec::starts_on(sched, fib_sender(fib_s{cutoff, n, sched}));
        };

        stdexec::start_detached(
          stdexec::when_all(mkchild(n - 1), mkchild(n - 2))
          | stdexec::then([rcvr = static_cast<Receiver&&>(rcvr_)](long a, long b) mutable {
              stdexec::set_value(static_cast<Receiver&&>(rcvr), a + b);
            }));
      }
    }
  };

  template <stdexec::receiver_of<completion_signatures> Receiver>
  friend auto tag_invoke(stdexec::connect_t, fib_s self, Receiver rcvr) -> operation<Receiver> {
    return {static_cast<Receiver&&>(rcvr), self.cutoff, self.n, self.sched};
  }
};

template <typename duration, typename F>
auto measure(F&& f) {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  f();
  return std::chrono::duration_cast<duration>(std::chrono::steady_clock::now() - start).count();
}




template <class Scheduler>
fib_s(long cutoff, long n, Scheduler sched) -> fib_s<Scheduler>;

//std::chrono::microseconds measure_time_stdexec(size_t num_threads, size_t num_fibonacci) {
size_t measure_time_stdexec(size_t num_threads, size_t num_fibonacci) {

  std::variant<execpools::tbb_thread_pool, exec::static_thread_pool> pool;

  pool.emplace<execpools::tbb_thread_pool>(static_cast<int>(num_threads));

  long result;

  size_t elapsed;

  for (size_t i = 0; i < 1; ++i) {
    auto snd = std::visit(
      [&](auto&& pool) { return fib_sender(fib_s{2, num_fibonacci, pool.get_scheduler()}); }, pool);

    auto time = measure<std::chrono::microseconds>(
      [&] { std::tie(result) = stdexec::sync_wait(std::move(snd)).value(); });
   
    elapsed = static_cast<size_t>(time); 
  }
  
  return elapsed;
}


