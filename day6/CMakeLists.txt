cmake_minimum_required(VERSION 3.16)

project(AdventOfCode2024_Day6 C)

set(CMAKE_C_STANDARD 99)

add_executable(day6_part1 src/main_part1.c)
add_executable(day6_part2 src/main_part2.c)

target_compile_options(day6_part1 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_options(day6_part2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)