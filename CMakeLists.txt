CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(AndroidWars)

# Set output directory
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_DIR})

find_package(Boost 1.34)
find_package(Qt4 COMPONENTS QtCore QtOpenGL QtGui REQUIRED)

# Optional parameters
set(CPPFLAGS "-g" CACHE STRING "Compiler flags to use")
set(LINKFLAGS "" CACHE STRING "Linker flags to use")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKFLAGS}")

# Add additional compiler flags
add_definitions(${CPPFLAGS})
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(simulation/)
add_subdirectory(pc/)

set(CMAKE_EXE_LINKER_FLAGS ${LINKFLAGS})