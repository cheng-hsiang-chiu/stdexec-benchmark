#include "integrate.hpp"


template <class... Ts>
using any_sender_of =
  typename exec::any_receiver_ref<stdexec::completion_signatures<Ts...>>::template any_sender<>;

using integrate_sender = any_sender_of<stdexec::set_value_t(double)>;

template <typename Scheduler>
struct integrate_s {
  using sender_concept = stdexec::sender_t;
  using completion_signatures = stdexec::completion_signatures<stdexec::set_value_t(double)>;

  double x1;
  double y1;
  double x2;
  double y2;
  double area = 0.0;
  Scheduler sched;

  template <class Receiver>
  struct operation {
    Receiver rcvr_;
    double x1;
    double y1;
    double x2;
    double y2;
    double area;
    Scheduler sched;

    void start() & noexcept {

      double half = (x2 - x1) / 2.0;
      double x0 = x1 + half;
      double y0 = fn(x0);

      double area_x1x0 = (y1 + y0) / 2 * half;
      double area_x0x2 = (y0 + y2) / 2 * half;
      double area_x1x2 = area_x1x0 + area_x0x2;
  
      if (area_x1x2 - area < 1e-9 && area - area_x1x2 < 1e-9) {
        return stdexec::set_value(static_cast<Receiver&&>(rcvr_), area_x1x2);
      }
      else {
        auto mkchild = [&](double x1, double y1, double x0, double y0, double area_x1x0) {
          return stdexec::starts_on(sched, integrate_sender(integrate_s{x1,y1,x0,y0,area_x1x0,sched}));
        };
        
        stdexec::start_detached(
          stdexec::when_all(mkchild(x1,y1,x0,y0,area_x1x0), mkchild(x0,y0,x2,y2,area_x0x2))
          | stdexec::then([rcvr = static_cast<Receiver&&>(rcvr_)](double area_x1x0, double area_x0x2) mutable {
              stdexec::set_value(static_cast<Receiver&&>(rcvr), area_x1x0 + area_x0x2);
            }));
      }
    }
  };

  template <stdexec::receiver_of<completion_signatures> Receiver>
  friend auto tag_invoke(stdexec::connect_t, integrate_s self, Receiver rcvr) -> operation<Receiver> {
    return {static_cast<Receiver&&>(rcvr), self.x1, self.y1, self.x2, self.y2, self.area, self.sched};
  }
};

template <class Scheduler>
integrate_s(double x1, double y1, double x2, double y2, double area, Scheduler sched) -> integrate_s<Scheduler>;

template <typename duration, typename F>
auto measure(F&& f) {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  f();
  return std::chrono::duration_cast<duration>(std::chrono::steady_clock::now() - start).count();
}

/*
auto main(int argc, char** argv) -> int {
  if (argc != 4) {
    std::cerr << "Usage: example.integrate n rounds tbb" << std::endl;
    return -1;
  }
  static constexpr size_t warmup = 1;
  double n = std::strtol(argv[1], nullptr, 10);
  size_t rounds = std::strtol(argv[2], nullptr, 10);

  std::variant<execpools::tbb_thread_pool, exec::static_thread_pool> pool;

  if (argv[3] == std::string_view("tbb")) {
    pool.emplace<execpools::tbb_thread_pool>(static_cast<int>(std::thread::hardware_concurrency()));
  } else {
    pool.emplace<exec::static_thread_pool>(
      std::thread::hardware_concurrency(), exec::bwos_params{}, exec::get_numa_policy());
  }

  std::vector<unsigned long> times;
  double result;
  
  for (unsigned long i = 0; i < rounds; ++i) {
    auto snd = std::visit(
      [&](auto&& pool) { return integrate_sender(integrate_s{0.0, fn(0.0), n, fn(n), 0.0, pool.get_scheduler()}); }, pool);

    auto time = measure<std::chrono::milliseconds>(
      [&] { std::tie(result) = stdexec::sync_wait(std::move(snd)).value(); });
    times.push_back(static_cast<unsigned int>(time));
  }

  std::cout << "Avg time: "
            << (std::accumulate(times.begin() + warmup, times.end(), 0u) / (times.size() - warmup))
            << "ms. Result: " << result << std::endl;
}
*/


size_t measure_time_stdexec(size_t num_threads, size_t max_value) {

  double result;

  size_t elapsed;

  std::variant<execpools::tbb_thread_pool, exec::static_thread_pool> pool;

  pool.emplace<execpools::tbb_thread_pool>(static_cast<int>(num_threads));
  
  for (unsigned long i = 0; i < 1; ++i) {
    auto snd = std::visit(
      [&](auto&& pool) { return integrate_sender(integrate_s{0.0, fn(0.0), max_value, fn(max_value), 0.0, pool.get_scheduler()}); }, pool);

    auto time = measure<std::chrono::microseconds>(
      [&] { std::tie(result) = stdexec::sync_wait(std::move(snd)).value(); });
    elapsed = static_cast<unsigned int>(time);
  }

  return elapsed;
}
