#include "reduce_sum.hpp"
#include <CLI11.hpp>

void bench_reduce_sum(
  const unsigned num_threads,
  const unsigned num_rounds) {

  std::cout << std::setw(12) << "size"
            << std::setw(12) << "runtime"
            << std::endl;

  for(size_t N = 10; N <= 1000000000; N = N * 10) {

    vec.resize(N);

    double runtime {0.0};

    for(unsigned j = 0; j < num_rounds; ++j) {
      runtime += measure_time_stdexec(num_threads).count();
    }

    std::cout << std::setw(12) << N
              << std::setw(12) << runtime / num_rounds / 1e3
              << std::endl;
  }
}

int main(int argc, char* argv[]) {

  CLI::App app{"Reduce_sum"};

  size_t num_threads {std::thread::hardware_concurrency()};
  app.add_option("-t,--num_threads", num_threads, 
                 "number of threads (default=hardware concurrency)");

  unsigned num_rounds {1};
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");

  CLI11_PARSE(app, argc, argv);

  std::cout << "num_threads=" << num_threads << ' '
            << "num_rounds=" << num_rounds << ' '
            << std::endl;

  bench_reduce_sum(num_threads, num_rounds);

  return 0;
}


