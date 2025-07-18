#include "fibonacci.hpp"
#include <CLI11.hpp>

void bench_fibonacci(
  const std::string& model,
  const unsigned num_threads,
  const unsigned num_rounds,
  const unsigned num_fibonacci) {

  std::cout << std::setw(12) << "size"
            << std::setw(12) << "runtime"
            << std::endl;

  for(size_t f = 1; f <= num_fibonacci; ++f) {

    double runtime {0.0};

    for(unsigned j = 0; j < num_rounds; ++j) {
      if (model == "stdexec") {
        runtime += measure_time_stdexec(num_threads, f); 
      }
      else if (model == "tbb") {
        runtime += measure_time_tbb(num_threads, f);
      }
      else assert(false);
    }

    std::cout << std::setw(12) << f
              << std::setw(12) << runtime / num_rounds / 1e3
              << std::endl;
  }
}


int main(int argc, char* argv[]) {

  CLI::App app{"Fibonacci"};

  size_t num_threads {std::thread::hardware_concurrency()};
  app.add_option("-t,--num_threads", num_threads, 
                 "number of threads (default=hardware concurrency)");

  unsigned num_rounds {1};
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");
  
  unsigned num_fibonacci {40};
  app.add_option("-f,--fibonacci", num_fibonacci, "max number of fibonacci (default=40)");
  
  std::string model = "stdexec";
  app.add_option("-m,--model", model, "model name stdexec|tbb (default=stdexec)")
     ->check([] (const std::string& m) {
        if(m != "stdexec" && m != "tbb") {
          return "model name should be \"tbb\" or \"stdexec\"";
        }
        return "";
     });

  CLI11_PARSE(app, argc, argv);

  std::cout << "model="       << model << ' '
            << "num_threads=" << num_threads << ' '
            << "num_rounds="  << num_rounds << ' '
            << "fibonacci="   << num_fibonacci << ' '
            << std::endl;

  bench_fibonacci(model, num_threads, num_rounds, num_fibonacci);

  return 0;
}


