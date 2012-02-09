add_library(AWExtSim
	ExtSim.cpp
	ExtData.cpp
	ExtInput.cpp
	BaseData.cpp
	TypeRule.cpp
	
	ExtProgramInput.cpp
	
	param/Position.cpp
	param/IdList.cpp
	
	typerule/program/ArmorAttachment.cpp
	
	datatype/Game.cpp
	datatype/Map.cpp
	datatype/Armor.cpp
	datatype/Damage.cpp
	datatype/Bot.cpp
	datatype/Program.cpp
	)
target_link_libraries(AWExtSim AWSim)

if(COMP_EXTSIM_TEST AND Boost_FILESYSTEM_FOUND)
	add_executable(../ExtSimTest test/exttest.cpp)
	target_link_libraries(../ExtSimTest AWSim AWExtSim DaScript ${Boost_FILESYSTEM_LIBRARY})
	
	add_definitions(-DDATADIR="${CMAKE_HOME_DIRECTORY}/data/")
endif(COMP_EXTSIM_TEST AND Boost_FILESYSTEM_FOUND)
