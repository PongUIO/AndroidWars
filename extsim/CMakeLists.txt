add_library(AWExtSim
	ExtSim.cpp
	ExtData.cpp
	ExtErrorMgr.cpp
	InputBarrier.cpp
	TypeRuleMgr.cpp
	TypeRuleLoader.cpp
	CommonTemplate.cpp
	CommonLoad.cpp
	
	simcontrol/ExtCpuInput.cpp
	
	agent/AgentMgr.cpp
	agent/Agent.cpp
	
	replay/ReplayTree.cpp
	replay/ReplayManager.cpp
	
	param/Position.cpp
	param/IdParam.cpp
	param/ValueParam.cpp
	
	object/ExtDataObj.cpp
	object/TypeRule.cpp
	object/TypeRuleObj.cpp
	object/ParamList.cpp
	
	typerule/bot/BaseBot.cpp
	typerule/program/MoveTowards.cpp
	typerule/program/Kill.cpp
	typerule/misc/CpuInput.cpp
	
	datatype/ExtGame.cpp
	datatype/ExtMap.cpp
	datatype/ExtArmor.cpp
	datatype/ExtDamage.cpp
	datatype/ExtBot.cpp
	datatype/ExtProgram.cpp
	datatype/ExtWeapon.cpp
	)
target_link_libraries(AWExtSim AWSim)

if(COMP_EXTSIM_TEST)
	set(ES_LIBS AWExtSim nepeta ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
	
	if(Boost_FILESYSTEM_FOUND)
		add_executable(../ExtSimTest test/exttest.cpp)
		add_executable(../ExtReplayTest test/replaytest.cpp)
		
		target_link_libraries(../ExtSimTest ${ES_LIBS})
		target_link_libraries(../ExtReplayTest ${ES_LIBS})
	endif()
	
	add_executable(../ConvTest test/convtest.cpp)
	
	add_definitions(-DDATADIR="${CMAKE_HOME_DIRECTORY}/testmod/data/")
endif()
