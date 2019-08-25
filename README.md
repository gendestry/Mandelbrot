# Mandelbrot
This program was written in C++ using OpenGL. The limit of the zoom is **64 bit**.

## Controls
To move around you can either use **AWSD** or drag with mouse.  
To zoom in and out you can either use **+/-** or the scroll wheel.  
To increase or decrease the number of iterations use **Q/E**.  
Press **NUMPAD 0** to reset the whole thing. 

## Building
Required libraries:
- glfw
- glew

To build it on linux use **g++ Source.cpp -lglfw -lGL -lGLEW -o Source**. Be sure to have te required libraries installed! 
