find_package(OpenAL)

set(COMP_SOUND true CACHE BOOL "Compile sound library")

if(OpenAL_FOUND AND COMP_SOUND)
	add_library(sound STATIC
		sound.cpp
		)

	set(SOUND_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}" PARENT_SCOPE)

	include_directories(${OPENAL_INCLUDE_DIR})
	target_link_libraries(sound ${OPENAL_LIBRARY})
	target_link_libraries(sound -lalure)
	add_executable(sndtest test/sndtest.cpp)
	target_link_libraries(sndtest sound)
endif()
