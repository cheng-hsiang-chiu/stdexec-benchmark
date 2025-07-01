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
#include <atomic>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>

inline std::vector<double> vec;

std::chrono::microseconds measure_time_stdexec(size_t);

