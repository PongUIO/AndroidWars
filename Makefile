all:
	rm -rf build
	mkdir build
	cd pc; qmake AndroidWars.pro; make; cp AndroidWars ../build/AndroidWarsPC
