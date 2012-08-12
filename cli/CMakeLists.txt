include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(cli
	main.cpp
	CmdManager.cpp
	Command.cpp

	command/Echo.cpp
	command/include.cpp
)

target_link_libraries(cli -lnepeta -lreadline)