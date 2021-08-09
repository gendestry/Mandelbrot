all:
	g++ `pkg-config --cflags glfw3 glew` -o Source Source.cpp `pkg-config --static --libs glfw3 glew`

run:
	./Source
