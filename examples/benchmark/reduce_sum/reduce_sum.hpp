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

#include <execpools/tbb/tbb_thread_pool.hpp>
#include <exec/static_thread_pool.hpp>

#include <exec/any_sender_of.hpp>
#include <stdexec/execution.hpp>


inline std::vector<double> vec;

std::chrono::microseconds measure_time_stdexec(unsigned);

