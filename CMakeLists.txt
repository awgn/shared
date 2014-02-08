cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Itests/ -I. -march=native -Wall -Wno-unused-variable -std=c++11")

include_directories(.)

add_executable(test-list     tests/test-shared_list.cpp)
add_executable(test-list-mt  tests/test-shared_list-mt.cpp)
add_executable(perf-list     tests/perf-shared_list.cpp)

add_executable(test-map      tests/test-shared_unordered_map.cpp)
add_executable(test-map-mt   tests/test-shared_unordered_map-mt.cpp)

target_link_libraries(test-list    -pthread)
target_link_libraries(test-list-mt -pthread)
target_link_libraries(perf-list    -lboost_system -lboost_thread)
target_link_libraries(test-map     -pthread)
target_link_libraries(test-map-mt  -pthread)
