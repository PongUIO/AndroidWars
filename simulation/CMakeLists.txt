CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(AWSimulation)

# Set output directory
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_DIR})

find_package(Boost 1.34)

# Optional parameters
set(CPPFLAGS "-g" CACHE STRING "Compiler flags to use")
set(LINKFLAGS "" CACHE STRING "Linker flags to use")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKFLAGS}")

# Add additional compiler flags
add_definitions(${CPPFLAGS})
include_directories(${Boost_INCLUDE_DIR})

add_library(AWSim
	Body.cpp
	Bot.cpp
	BotInput.cpp
	Data.cpp
	Side.cpp
	Simulation.cpp
	State.cpp
	Sync.cpp
	World.cpp
	
	collision/Collision.cpp
	collision/TileCol.cpp
	)

add_executable(SimTest
	test/simtest.cpp)

target_link_libraries(SimTest AWSim)

set(CMAKE_EXE_LINKER_FLAGS ${LINKFLAGS})
