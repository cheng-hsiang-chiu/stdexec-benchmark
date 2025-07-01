#pragma once

#include <algorithm> // for std::max
#include <cassert>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <random>
#include <cmath>
#include <vector>

#include <execpools/tbb/tbb_thread_pool.hpp>
#include <exec/static_thread_pool.hpp>

#include <exec/any_sender_of.hpp>
#include <stdexec/execution.hpp>

using Solution = std::vector<int>;
using SolutionSet = std::vector<Solution>;
using sol_sender = exec::any_receiver_ref<
    stdexec::completion_signatures<
        stdexec::set_value_t(SolutionSet),
        stdexec::set_error_t(std::exception_ptr),
        stdexec::set_stopped_t()
    >
>::any_sender<>;

std::chrono::microseconds measure_time_stdexec(size_t, size_t);

inline std::vector<unsigned long long int> answers{0,1,0,0,2,10,4,40,92,352,724,2680,14200,73712,365596,2279184,14772512,95815104,666090624};

//inline auto queens_ok(int n, char* a) -> bool {
//
//  for (int i = 0; i < n; i++) {
//
//    char p = a[i];
//
//    for (int j = i + 1; j < n; j++) {
//      if (char q = a[j]; q == p || q == p - (j - i) || q == p + (j - i)) {
//        return false;
//      }
//    }
//  }
//  return true;
//}


// Check if placing a queen is valid
inline bool is_valid(const Solution& partial, int col) {
  int row = partial.size();
  for (int r = 0; r < row; ++r) {
    if (partial[r] == col || abs(partial[r] - col) == row - r) {
      return false;
    }
  }
  return true;
}
