#include "GLFW\glfw3.h"
#include "stdio.h"
#include "loadFrame.cpp"

int main(int argv, char* argc[]) {
	GLFWwindow* window;
	if(!glfwInit()) {
		printf("ERROR: GLFW init failed\n");
		return 1;
	}
	
	window = glfwCreateWindow(1280, 720, "Video Player", NULL, NULL);
	if(!window) {
		printf("ERROR: GLFW createWindow failed\n");
		return 1;
	}

	int frameWidth, frameHeight;
	unsigned char* frameData;
	const char* videoPath = "c:/users/fefe/Pictures/SAINT LAURENT - FALL 2020 - ROSÉ-E9m4IzTEbA0.mkv";
	if(!loadFrame(videoPath, &frameWidth, &frameHeight, &frameData)) {
		printf("ERROR: loadFrame failed\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	GLuint textureHandle;
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, frameData);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int windowWidth, windowHeight;
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);

		/* render */
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0); glVertex2i(0, 0);
			glTexCoord2d(1, 0); glVertex2i(0 + frameWidth, 0);
			glTexCoord2d(1, 1); glVertex2i(0 + frameWidth, 0 + frameHeight);
			glTexCoord2d(0, 1); glVertex2i(0, 0 + frameHeight);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glfwSwapBuffers(window);
		glfwWaitEvents();

	}
	return 0;
}