#include <GLFW/glfw3.h>
#include "stdio.h"

int main(int argv, char* argc[]) {
	GLFWwindow* window;
	if(!glfwInit()) {
		printf("ERROR; GLFW init failed\n");
		return 1;
	}
	
	window = glfwCreateWindow(640, 480, "Video app", NULL, NULL);
	if(!window) {
		printf("ERROR; GLFW createWindow failed\n");
		return 1;
	}

	glfwMakeContextCurrent(window);
	while(!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
	}
	return 0;
}