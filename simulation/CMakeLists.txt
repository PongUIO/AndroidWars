add_library(AWSim
	Body.cpp
	Bot.cpp
	BotInput.cpp
	Data.cpp
	Player.cpp
	Bullet.cpp
	Weapon.cpp
	Simulation.cpp
	State.cpp
	Save.cpp
	World.cpp
	
	bot/BotCpu.cpp
	
	data/BotD.cpp
	data/BulletD.cpp
	data/WeaponD.cpp
	
	collision/Collision.cpp
	collision/TileCol.cpp
	
	statesys/StateSys.cpp
	statesys/Reference.cpp
	)

add_executable(../SimTest
	test/simtest.cpp)

add_executable(../StateTest
	test/statetest.cpp)

target_link_libraries(../SimTest AWSim)
target_link_libraries(../StateTest AWSim)

