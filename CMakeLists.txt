CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(AndroidWars)

# Set output directory
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_DIR})

#find_package(Boost 1.34 COMPONENTS filesystem)
find_package(Qt4 COMPONENTS QtCore QtOpenGL QtGui REQUIRED)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation" VERBATIM)
endif(DOXYGEN_FOUND)

# Optional parameters
set(CPPFLAGS "-g" CACHE STRING "Compiler flags to use")
set(LINKFLAGS "" CACHE STRING "Linker flags to use")

set(COMP_SIM_TEST false CACHE BOOL "Compile simulation tests")
set(COMP_TIME_TEST false CACHE BOOL "Compile timeline manager tests")
set(COMP_EXTSIM_TEST false CACHE BOOL "Compile extsim tests")
set(COMP_GUI true CACHE BOOL "Compile GUI")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKFLAGS}")

# Add additional compiler flags
add_definitions(${CPPFLAGS})
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(nepeta/)

include_directories(${NEPETA_INCLUDE_DIR})

add_subdirectory(simulation/)
add_subdirectory(timeline/)
add_subdirectory(extsim/)
add_subdirectory(sound/)
add_subdirectory(cli/)

if(COMP_GUI)
	add_subdirectory(pc/)
endif(COMP_GUI)

set(CMAKE_EXE_LINKER_FLAGS ${LINKFLAGS})
