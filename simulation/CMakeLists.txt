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
	Player.cpp
	Bullet.cpp
	Weapon.cpp
	Simulation.cpp
	State.cpp
	Save.cpp
	World.cpp
	
	data/BotD.cpp
	data/BulletD.cpp
	data/WeaponD.cpp
	
	collision/Collision.cpp
	collision/TileCol.cpp
	
	statesys/StateSys.cpp
	statesys/Reference.cpp
	)

add_executable(SimTest
	test/simtest.cpp)

add_executable(StateTest
	test/statetest.cpp)

target_link_libraries(SimTest AWSim)
target_link_libraries(StateTest AWSim)

set(CMAKE_EXE_LINKER_FLAGS ${LINKFLAGS})
