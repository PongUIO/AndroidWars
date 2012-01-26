add_library(AWTimelineMgr
	TimelineMgr.cpp
	)

if(COMP_TIME_TEST)
	add_executable(../SimTest test/simtest.cpp)
	target_link_libraries(../SimTest AWSim)
endif(COMP_TIME_TEST)
