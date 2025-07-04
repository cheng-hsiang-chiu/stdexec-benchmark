#=============================================================================
# Copyright 2023 NVIDIA Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#=============================================================================

function(split pair name_out path_out)
    string(STRIP "${pair}" pair)
    string(REPLACE ":" ";" pair "${pair}")
    list(POP_FRONT pair _name)
    list(POP_FRONT pair _path)
    string(STRIP "${_name}" _name)
    string(STRIP "${_path}" _path)
    set(${name_out} "${_name}" PARENT_SCOPE)
    set(${path_out} "${_path}" PARENT_SCOPE)
endfunction()

function(def_example example)
    split(${example} target source)
    add_executable(${target} ${source})
    set_target_properties(${target} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF)
    target_link_libraries(${target}
        PRIVATE STDEXEC::stdexec
                stdexec_executable_flags)
endfunction()

set(stdexec_examples
               "example.clangd.helper : _clangd_helper_file.cpp"
                 "example.hello_world : hello_world.cpp"
                  "example.hello_coro : hello_coro.cpp"
                       "example.scope : scope.cpp"
                       "example.retry : retry.cpp"
                        "example.then : then.cpp"
      "example.server_theme.let_value : server_theme/let_value.cpp"
    "example.server_theme.on_transfer : server_theme/on_transfer.cpp"
      "example.server_theme.then_upon : server_theme/then_upon.cpp"
     "example.server_theme.split_bulk : server_theme/split_bulk.cpp"
"example.benchmark.static_thread_pool : benchmark/static_thread_pool.cpp"
"example.benchmark.static_thread_pool_old : benchmark/static_thread_pool_old.cpp"
"example.benchmark.static_thread_pool_nested : benchmark/static_thread_pool_nested.cpp"
"example.benchmark.static_thread_pool_nested_old : benchmark/static_thread_pool_nested_old.cpp"
"example.benchmark.static_thread_pool_bulk_enqueue : benchmark/static_thread_pool_bulk_enqueue.cpp"
"example.benchmark.static_thread_pool_bulk_enqueue_nested : benchmark/static_thread_pool_bulk_enqueue_nested.cpp"
)

if (LINUX)
  set(stdexec_examples ${stdexec_examples}
                    "example.io_uring : io_uring.cpp"
  )
endif (LINUX)

foreach(example ${stdexec_examples})
    def_example(${example})
endforeach()

if(STDEXEC_ENABLE_CUDA)
    add_subdirectory(nvexec)
endif()

if (STDEXEC_ENABLE_TBB)
 add_executable(example.benchmark.tbb_thread_pool benchmark/tbb_thread_pool.cpp)
 target_link_libraries(example.benchmark.tbb_thread_pool PRIVATE STDEXEC::tbbpool)

 add_executable(example.benchmark.tbb_thread_pool_nested benchmark/tbb_thread_pool_nested.cpp)
 target_link_libraries(example.benchmark.tbb_thread_pool_nested PRIVATE STDEXEC::tbbpool)

 add_executable(example.benchmark.fibonacci benchmark/fibonacci.cpp)
 target_link_libraries(example.benchmark.fibonacci PRIVATE STDEXEC::tbbpool)
endif()

if(STDEXEC_ENABLE_TASKFLOW)
 add_executable(example.benchmark.taskflow_thread_pool benchmark/taskflow_thread_pool.cpp)
 target_link_libraries(example.benchmark.taskflow_thread_pool PRIVATE STDEXEC::taskflow_pool)
endif()

if(STDEXEC_ENABLE_ASIO)
add_executable(example.benchmark.asio_thread_pool benchmark/asio_thread_pool.cpp)
target_link_libraries(example.benchmark.asio_thread_pool PRIVATE STDEXEC::asio_pool)
endif()




# for_each benchmark
add_executable(
  benchmark.for_each 
  benchmark/for_each/main.cpp
  benchmark/for_each/stdexec.cpp)
target_link_libraries(
  benchmark.for_each
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
)

# fibonacci benchmark
add_executable(
  benchmark.fibonacci
  benchmark/fibonacci/main.cpp
  benchmark/fibonacci/tbb.cpp
  benchmark/fibonacci/stdexec.cpp)
target_link_libraries(
  benchmark.fibonacci
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)

# linear_chain benchmark
add_executable(
  benchmark.linear_chain
  benchmark/linear_chain/main.cpp
  benchmark/linear_chain/stdexec.cpp)
target_link_libraries(
  benchmark.linear_chain
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
)

# integrate benchmark
add_executable(
  benchmark.integrate
  benchmark/integrate/main.cpp
  benchmark/integrate/tbb.cpp
  benchmark/integrate/stdexec.cpp)
target_link_libraries(
  benchmark.integrate
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)

# primes benchmark
add_executable(
  benchmark.primes
  benchmark/primes/main.cpp
  benchmark/primes/tbb.cpp
  benchmark/primes/stdexec.cpp)
target_link_libraries(
  benchmark.primes
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)

# reduce_sum benchmark
add_executable(
  benchmark.reduce_sum
  benchmark/reduce_sum/main.cpp
  benchmark/reduce_sum/stdexec.cpp)
target_link_libraries(
  benchmark.reduce_sum
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)

# sort benchmark
add_executable(
  benchmark.sort
  benchmark/sort/main.cpp
  benchmark/sort/stdexec.cpp)
target_link_libraries(
  benchmark.sort
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)

# nqueens benchmark
add_executable(
  benchmark.nqueens
  benchmark/nqueens/main.cpp
  benchmark/nqueens/tbb.cpp
  benchmark/nqueens/stdexec.cpp)
target_link_libraries(
  benchmark.nqueens
  PRIVATE STDEXEC::stdexec
          stdexec_executable_flags
          STDEXEC::tbbpool
)
