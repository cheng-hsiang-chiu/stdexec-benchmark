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



size_t measure_time_stdexec(size_t num_threads, size_t max_value) {

  double result;

  size_t elapsed;

  std::variant<execpools::tbb_thread_pool, exec::static_thread_pool> pool;

  pool.emplace<execpools::tbb_thread_pool>(static_cast<int>(num_threads));
  
  auto snd = std::visit(
    [&](auto&& pool) { return integrate_sender(integrate_s{0.0, fn(0.0), max_value, fn(max_value), 0.0, pool.get_scheduler()}); }, pool);

  auto time = measure<std::chrono::microseconds>(
    [&] { std::tie(result) = stdexec::sync_wait(std::move(snd)).value(); });
  elapsed = static_cast<unsigned int>(time);

  return elapsed;
}
