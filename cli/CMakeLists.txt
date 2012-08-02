CMAKE_MINIMUM_REQUIRED( VERSION 2.6)
project(aw-cli)

find_package(Nepeta REQUIRED)

include_directories(${NEPETA_INCLUDE_DIR})
add_subdirectory(src/)