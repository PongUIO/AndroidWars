add_library(AWSim
	Body.cpp
	Bot.cpp
	Input.cpp
	Program.cpp
	Ability.cpp
	Data.cpp
	Player.cpp
	Bullet.cpp
	Weapon.cpp
	Simulation.cpp
	State.cpp
	Save.cpp
	World.cpp
	
	bot/BotCpu.cpp
	bot/BotAbility.cpp
	
	program/MoveTowards.cpp
	program/Kill.cpp
	
	ability/CpuBoost.cpp
	
	data/BotD.cpp
	data/BulletD.cpp
	data/WeaponD.cpp
	data/ProgramD.cpp
	data/AbilityD.cpp
	
	collision/Collision.cpp
	collision/TileCol.cpp
	)

add_executable(../SimTest
	test/simtest.cpp)

target_link_libraries(../SimTest AWSim)
