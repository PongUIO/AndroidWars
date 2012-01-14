add_library(AWSim
	Body.cpp
	Bot.cpp
	Input.cpp
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
	Replay.cpp
	
	bot/BotCpu.cpp
	bot/BotAbility.cpp
	bot/BotWeapon.cpp
	
	program/MoveTowards.cpp
	program/Kill.cpp
	program/Sensor.cpp
	program/Shoot.cpp
	
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
