#include "Global.h"
#include "Window.h"
#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello");

	window.Open();
	window.InitGL();

	const float triangleVertices[] = {
		0.0f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.366f, 0.0f, 1.0f,
		-0.5f, -0.366f, 0.0f, 1.0f,
		//next part contains vertex colors
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	}; //we love you vertices!

	Shader sh;
	sh.Comiple("shaders/vs1.glsl", "shaders/fs1.glsl");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); //make our vertex array object, we need it to restore state we set after binding it. Re-binding reloads the state associated with it.

	GLuint triangleBufferObject;
	glGenBuffers(1, &triangleBufferObject); //create the buffer
	glBindBuffer(GL_ARRAY_BUFFER, triangleBufferObject); //we're "using" this one now
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //formatting the data for the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind any buffers

	printf("glError: %d\n", glGetError());

	char bGameLoopRunning = 1;
	while (bGameLoopRunning)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				bGameLoopRunning = 0;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				bGameLoopRunning = 0;
		}

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* drawing code in here! */
		glUseProgram(sh.GetID());
		glBindBuffer(GL_ARRAY_BUFFER, triangleBufferObject); //bind the buffer we're applying attributes to
		glEnableVertexAttribArray(0); //0 is our index, refer to "location = 0" in the vertex shader
		glEnableVertexAttribArray(1); //attribute 1 is for vertex color data
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //tell gl (shader!) how to interpret our vertex data
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48); //color data is 48 bytes in to the array
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(0);
		/* drawing code above here! */

		window.SwapBuffers();
		SDL_Delay(20);
	}

	window.DestoryGL();
	SDL_Quit();
	return 0;
}