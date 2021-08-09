.PHONY: all run clean

SOURCE := Source.cpp
BINARY := Mandelbrot
DEPS   := glfw3 glew

CFLAGS := -Wall
#LIBS   := $$(pkg-config --static --cflags --libs $(DEPS))
LIBS   := $$(pkg-config --cflags --libs $(DEPS))

all: $(BINARY)

$(BINARY): $(SOURCE)
	$(CXX) $(CFLAGS) $(LIBS) -o $@ $^

run:
	./$(BINARY)

clean:
	rm -f ./$(BINARY)
