add_library(AWExtSim
	ExtSim.cpp
	ExtData.cpp
	InputBarrier.cpp
	TypeRuleMgr.cpp
	TypeRuleLoader.cpp
	CommonTemplate.cpp
	CommonLoad.cpp
	
	simcontrol/ExtCpuInput.cpp
	
	replay/ReplayTree.cpp
	replay/ReplayManager.cpp
	
	param/Position.cpp
	param/IdList.cpp
	
	object/ExtDataObj.cpp
	object/TypeRule.cpp
	object/TypeRuleObj.cpp
	object/ParamList.cpp
	
	typerule/program/MoveTowards.cpp
	typerule/misc/CpuInput.cpp
	
	#typerule/bot/DefaultBot.cpp
	
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
		target_link_libraries(../ExtSimTest ${ES_LIBS})
	endif()
	
	add_executable(../ConvTest test/convtest.cpp)
	
	add_definitions(-DDATADIR="${CMAKE_HOME_DIRECTORY}/testmod/data/")
endif()
