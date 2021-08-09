# Mandelbrot
This program was written in C++ using OpenGL. The limit of the zoom is **64 bit**.

## Controls
To move around, use either **WASD** or drag with the mouse.  
To zoom in and out, use either **+/-** or the scroll wheel.  
To increase or decrease the number of iterations, use **Q/E**.  
Press **NUMPAD 0** to reset the whole thing.
To exit, press **ESC**.

## Building
Required libraries:
- GLFW
- glew

A Makefile is provided for program compilation. Simply running `make` should be enough to compile the executable. `pkg-config` is used for dependency detection. Be sure to have the required libraries installed! 
