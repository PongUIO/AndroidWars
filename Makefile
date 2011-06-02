all:
	make -B pc


pc:
	rm -rf build
	mkdir build
	cd pc; qmake; make; cp pc ../build/AndroidWarsPC

clean:
	rm -rf build
