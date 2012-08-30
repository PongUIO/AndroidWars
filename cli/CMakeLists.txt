include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(cli
	main.cpp
	CmdManager.cpp
	Command.cpp
	SimMgr.cpp

	command/Echo.cpp
	command/include.cpp
	command/startup.cpp
	command/shutdown.cpp
	command/runstep.cpp
	command/runphase.cpp
)

target_link_libraries(cli -lnepeta -ltecla AWExtSim)