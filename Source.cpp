#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

const int WIDTH  = 800;
const int HEIGHT = 600;

int itr = 200;
double zoom = 100.0;
double offsetX = 0.0;
double offsetY = 0.0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main() {
	if (!glfwInit()) LOG("GLFW: failed to init")
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Set", NULL, NULL);
	
	if (!window) {
		LOG("GLFW: failed to create the window")
		glfwTerminate();
	}
	
	glfwSetErrorCallback([](int e, const char *s) { std::cerr << s << std::endl; });
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwMakeContextCurrent(window);

	if (glewInit() != 0) LOG("GLEW: failed to init")
	
	float vertices[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,

		 1.0f,  1.0f,
		-1.0f,  1.0f,
		-1.0f, -1.0f
	};

	const char* vertexSource =
	"#version 410 core                      \n"
	"layout(location = 0) in vec2 pos;	    \n"
	"                                       \n"
	"void main() {                          \n"
	"	gl_Position = vec4(pos,0.0,1.0);    \n"
	"}                                      \n";

	const char* fragmentSource =
	"#version 410 core														\n"
	"																		\n"
	"uniform int itr;														\n"
	"uniform double zoom;													\n"
	"uniform dvec2 screenSize;												\n"
	"uniform dvec2 offset;													\n"
	"																		\n"
	"double n = 0.0;														\n"
	"double threshold = 100.0;												\n"
	"																		\n"
	"double mandelbrot(dvec2 c) {											\n"
	"	dvec2 z = vec2(0.0,0.0);											\n"
	"	for(int i = 0; i < itr; i++){										\n"
	"		dvec2 znew;														\n"
	"		znew.x = (z.x * z.x) - (z.y * z.y) + c.x;						\n"
	"		znew.y = (2.0 * z.x * z.y) +c.y;								\n"
	"		z = znew;														\n"
	"		if((z.x * z.x) + (z.y * z.y) > threshold)break;					\n"
	"		n++;															\n"
	"	}																	\n"
	"	return n / float(itr);												\n"
	"}																		\n"
	"																		\n"
	"vec4 map_to_color(float t) {											\n"
	"	float r = 9.0 * (1.0 - t) * t * t * t;								\n"
	"	float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;						\n"
	"	float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;				\n"
	"																		\n"
	"	return vec4(r, g, b, 1.0);											\n"
	"}																		\n"
	"																		\n"
	"void main() {															\n"
	"	dvec2 coord = dvec2(gl_FragCoord.xy);								\n"
	"	double t = mandelbrot(((coord - screenSize/2)/zoom) - offset);	    \n"
	"	if(gl_FragCoord.x < 40){											\n"
	"		gl_FragColor = vec4(1.0);										\n"
	"	}																	\n"
	"	gl_FragColor = map_to_color(float(t));								\n"
	"}																		\n";

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	unsigned int vertex, fragment, program;
	int success;
	char infoLog[1024];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: VERTEX\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: FRAGMENT\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glUseProgram(program);
	
	glUniform2d(glGetUniformLocation(program, "screenSize"), (double)WIDTH, (double)HEIGHT);
	glUniform2d(glGetUniformLocation(program, "offset"), offsetX, offsetY);
	glUniform1d(glGetUniformLocation(program, "zoom"), zoom);
	glUniform1i(glGetUniformLocation(program, "itr"), itr);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUniform1i(glGetUniformLocation(program, "itr"), itr);
		glUniform1d(glGetUniformLocation(program, "zoom"), zoom);
		glUniform2d(glGetUniformLocation(program, "offset"), offsetX, offsetY);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(program);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
	}

	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		zoom *= 2;
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		zoom /= 2;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		offsetX += 20 / zoom;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		offsetX -= 20 / zoom;
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		offsetY -= 20 / zoom;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		offsetY += 20 / zoom;

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		itr += 50;
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		(itr > 100) ? itr -= 50 : itr = 50;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset != 0) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//LOG(xpos)
		//LOG(ypos)

		double dx = (xpos - WIDTH / 2) / zoom - offsetX;
		double dy = (HEIGHT - ypos - HEIGHT / 2) / zoom - offsetY;
		offsetX = -dx;
		offsetY = -dy;
		if (yoffset < 0)
			zoom /= 1.2;
		else
			zoom *= 1.2;

		dx = (xpos - WIDTH / 2) / zoom;
		dy = (HEIGHT - ypos - HEIGHT / 2) / zoom;
		offsetX += dx;
		offsetY += dy;
	}
}
