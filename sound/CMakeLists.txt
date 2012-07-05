add_library(sound STATIC
    sound.cpp
    )

set(SOUND_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}" PARENT_SCOPE)

find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
target_link_libraries(sound ${OPENAL_LIBRARY})
target_link_libraries(sound -lalure)
add_executable(sndtest test/sndtest.cpp)
target_link_libraries(sndtest sound)

