#include "primes.hpp"
#include <CLI11.hpp>

void bench_primes(
  const unsigned num_threads,
  const unsigned num_rounds) {

  std::cout << std::setw(12) << "size"
            << std::setw(12) << "runtime"
            << std::endl;

  double runtime {0.0};

  for (size_t p = 10; p <= primes_limit; p *= 10) {
    runtime += measure_time_stdexec(num_threads, p).count();

    std::cout << std::setw(12) << p 
              << std::setw(12) << runtime / num_rounds / 1e3
              << std::endl;
  }  
}

int main(int argc, char* argv[]) {

  CLI::App app{"Primes"};

  size_t num_threads {std::thread::hardware_concurrency()};
  app.add_option("-t,--num_threads", num_threads, 
                 "number of threads (default=hardware concurrency)");
  
  unsigned num_rounds {1};
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");

  CLI11_PARSE(app, argc, argv);

  std::cout << "num_threads=" << num_threads << ' '
            << "num_rounds="  << num_rounds << ' '
            << "primes_limit="   << primes_limit << ' '
            << std::endl;

  bench_primes(num_threads, num_rounds);

  return 0;
}


