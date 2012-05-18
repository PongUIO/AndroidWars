set(DaScript_FILES dascript.cpp node.cpp dacodec.cpp writer.cpp daparser.cpp)
add_library(DaScript STATIC ${DaScript_FILES})

set(DASCRIPT_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}" PARENT_SCOPE)
