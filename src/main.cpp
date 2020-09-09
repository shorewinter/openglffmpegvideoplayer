#include <GLFW/glfw3.h>
#include "stdio.h"

int main(int argv, char* argc[]) {
	GLFWwindow* window;
	if(!glfwInit()) {
		printf("ERROR; GLFW init failed\n");
		return 1;
	}
	
	window = glfwCreateWindow(960, 540, "Video Player", NULL, NULL);
	if(!window) {
		printf("ERROR; GLFW createWindow failed\n");
		return 1;
	}

	unsigned char data[100 * 100 * 3];
	// unsigned char* data = new unsigned char[100 * 100 * 3]; /* cpp style */
	for(int y = 0; y < 100; ++y) {
		for(int x = 0; x < 100; ++x) {
			data[y * 100 * 3 + x * 3] = 0xff;
			data[y * 100 * 3 + x * 3 + 1] = 0x00;
			data[y * 100 * 3 + x * 3 + 2] = 0x00;
		}
	}

	for(int y = 25; y < 75; ++y) {
		for(int x = 25; x < 75; ++x) {
			data[y * 100 * 3 + x * 3] = 0x00;
			data[y * 100 * 3 + x * 3 + 1] = 0x00;
			data[y * 100 * 3 + x * 3 + 2] = 0xff;
		}
	}
	
	glfwMakeContextCurrent(window);

	GLuint textureHandle;
	int textureWidth = 100;
	int textureHeight = 100;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int windowWidth, windowHeight;
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
		glMatrixMode(GL_MODELVIEW);

		/* render */
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2i(200, 200);
			glTexCoord2d(1, 0); glVertex2i(200 + textureWidth * 2, 200);
			glTexCoord2d(1, 1); glVertex2i(200 + textureWidth * 2, 200 + textureHeight * 2);
			glTexCoord2d(0, 1); glVertex2i(200, 200 + textureHeight * 2);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glfwSwapBuffers(window);
		glfwWaitEvents();

	}
	return 0;
}