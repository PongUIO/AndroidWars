all:
	rm -rf build
	mkdir build
	cd pc; qmake; make; cp pc ../build/AndroidWarsPC
