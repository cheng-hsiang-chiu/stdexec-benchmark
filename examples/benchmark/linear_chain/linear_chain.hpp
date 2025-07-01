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


#include <exec/any_sender_of.hpp>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>

std::chrono::microseconds measure_time_stdexec(size_t, unsigned);

