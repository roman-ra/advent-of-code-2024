cmake_minimum_required(VERSION 3.16)

project(AdventOfCode2024_Day3 CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(day3_part1 src/main_part1.cpp)
add_executable(day3_part2 src/main_part2.cpp)

target_compile_options(day3_part1 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_options(day3_part2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)