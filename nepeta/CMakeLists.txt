add_library(nepeta STATIC
	nepeta.cpp
	node.cpp
	nepcodec.cpp
	writer.cpp
	nepparser.cpp
)

set(NEPETA_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}" PARENT_SCOPE)
