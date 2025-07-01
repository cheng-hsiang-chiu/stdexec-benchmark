#include "linear_chain.hpp"
#include <CLI11.hpp>

void bench_linear_chain(
  const size_t log_length,
  const unsigned num_threads,
  const unsigned num_rounds) {

  std::cout << std::setw(12) << "length"
            << std::setw(12) << "runtime"
            << std::endl;

  for(size_t i = 1; i <= log_length; ++i) {

    size_t L = 1 << i;

    double runtime {0.0};

    for(unsigned j = 0; j < num_rounds; ++j) {
      runtime += measure_time_stdexec(L, num_threads).count();
    }

    std::cout << std::setw(12) << L
              << std::setw(12) << runtime / num_rounds / 1e3
              << std::endl;
  }
}

int main(int argc, char* argv[]) {

  CLI::App app{"LinearChain"};

  size_t num_threads {std::thread::hardware_concurrency()};
  app.add_option("-t,--num_threads", num_threads, 
                 "number of threads (default=hardware concurrency)");

  unsigned num_rounds {1};
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");

  size_t log_length {25};
  app.add_option("-l,--log_length", log_length, "length in log scale (default=25)");

  CLI11_PARSE(app, argc, argv);

  std::cout << "num_threads=" << num_threads << ' '
            << "num_rounds=" << num_rounds << ' '
            << std::endl;

  bench_linear_chain(log_length, num_threads, num_rounds);

  return 0;
}


