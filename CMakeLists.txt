CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
project(AndroidWars)

# Setup doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation" VERBATIM)
else()
	add_custom_target(doc
		COMMENT "Doxygen is not installed")
endif()


# Add core libraries
find_package(Boost 1.34 COMPONENTS filesystem system)
add_subdirectory(nepeta/src/)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${NEPETA_INCLUDE_DIR})


# Setup basic parameters
set(COMP_EXTSIM_TEST false CACHE BOOL "Compile extsim tests")

# Add subcomponents
add_subdirectory(simulation/)
add_subdirectory(extsim/)
add_subdirectory(sound/)
add_subdirectory(pc/)
