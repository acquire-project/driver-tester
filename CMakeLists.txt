cmake_minimum_required(VERSION 3.5)
project(driver-tester)

set(NOTEST "TRUE")
add_subdirectory(acquire-core-libs)
set(NOTEST "FALSE")

add_subdirectory(src)
