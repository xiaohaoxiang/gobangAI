start : ai.h ai.cpp board.h board.cpp defs.h keys.h keys.cpp main.cpp point.h
	g++ keys.cpp board.cpp ai.cpp main.cpp -o start -std=c++11 -O3

.PHONY : clean
clean :
	-rm start