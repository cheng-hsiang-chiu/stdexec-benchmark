#pragma once

#include <algorithm> 
#include <cassert>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>
#include <random>
#include <cmath>
#include <vector>
#include <exec/any_sender_of.hpp>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <execpools/tbb/tbb_thread_pool.hpp>

size_t measure_time_stdexec(size_t, size_t);
size_t measure_time_tbb(size_t, size_t);

inline constexpr int n = 10000;
inline constexpr double epsilon = 1.0e-9;

inline constexpr auto fn(double x) -> double { return (x * x + 1.0) * x; }

//inline constexpr auto integral_fn(double a, double b) -> double {
//
//  constexpr auto indefinite = [](double x) {
//    return 0.25 * x * x * (x * x + 2);
//  };
//
//  return indefinite(b) - indefinite(a);
//}

