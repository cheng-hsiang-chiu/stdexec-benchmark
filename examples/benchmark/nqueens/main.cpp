#include "nqueens.hpp"
#include <CLI11.hpp>

void bench_nqueens(
  const std::string& model,
  const size_t num_threads,
  const size_t num_rounds,
  const size_t num_queens) {

  std::cout << std::setw(12) << "size"
            << std::setw(12) << "runtime"
            << std::endl;

  for(size_t q = 1; q <= num_queens; ++q) {

    double runtime {0.0};

    for(unsigned j = 0; j < num_rounds; ++j) {
      if (model == "stdexec") {
        runtime += measure_time_stdexec(num_threads, q).count();
      }
      else if (model == "tbb") {
        runtime += measure_time_tbb(num_threads, q).count();
      }

      else assert(false);
    }

    std::cout << std::setw(12) << q
              << std::setw(12) << runtime / num_rounds / 1e3
              << std::endl;
  }
}

int main(int argc, char* argv[]) {

  CLI::App app{"Nqueens"};

  size_t num_threads {std::thread::hardware_concurrency()};
  app.add_option("-t,--num_threads", num_threads, 
                 "number of threads (default=hardware concurrency)");
  
  size_t num_queens {14};
  app.add_option("-q,--queens", num_queens, "max number of queens (default=14)");

  size_t num_rounds {1};
  app.add_option("-r,--num_rounds", num_rounds, "number of rounds (default=1)");

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
            << "nqueens="   << num_queens << ' '
            << std::endl;

  bench_nqueens(model, num_threads, num_rounds, num_queens);

  return 0;
}


