set(COMP_SIM_TEST false CACHE BOOL "Compile simulation tests")

add_library(AWSim
	Body.cpp
	Bot.cpp
	Program.cpp
	Ability.cpp
	Health.cpp
	Data.cpp
	Player.cpp
	Bullet.cpp
	Weapon.cpp
	Simulation.cpp
	State.cpp
	Save.cpp
	World.cpp
	CommonTemplate.cpp
	
	bot/BotCpu.cpp
	bot/BotAbility.cpp
	bot/BotWeapon.cpp
	
	program/MoveTowards.cpp
	program/Kill.cpp
	program/Sensor.cpp
	program/Shoot.cpp
	
	ability/CpuBoost.cpp
	ability/ArmorAttachment.cpp
	
	data/BotD.cpp
	data/BulletD.cpp
	data/WeaponD.cpp
	data/ProgramD.cpp
	data/AbilityD.cpp
	
	collision/Collision.cpp
	collision/TileCol.cpp
	)

if(COMP_SIM_TEST)
	add_executable(../SimTest test/simtest.cpp)
	target_link_libraries(../SimTest AWSim)
endif(COMP_SIM_TEST)
