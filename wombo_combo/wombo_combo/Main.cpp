//TODO:
//1. Generate a grid 1024 * 1024
//2. Use a hightmap texture to replace y
//3. Generate normals 
//4. Generate UV
//5. Apply a Terrian Texture to it

#include "Global.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "MyTimer.h"
#include "FreeCamera.h"
#include "Terrian.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

//Put mouse in the center
GLfloat lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Shader sh;
MyTimer timer;
FreeCamera camera(point3(-1.0f, 0.0f, 1.5f));
Terrian terrian(1024, 1024);

void CameraMotion(GLfloat xpos, GLfloat ypos, FreeCamera* cam){
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		SDL_ShowCursor(SDL_DISABLE);
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	cam->Rotate(PITCH, yoffset);
	cam->Rotate(YAW, xoffset);
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedural Terrian");

	window.Open();
	window.InitGL();

	sh.Comiple("shaders/vs.glsl", "shaders/fs.glsl");
	timer.Reset();



	//InitData()
	GLuint VAO, VBO, EBO;
	terrian.GenTerrian("Assets/Terrian/height_map.jpg", true);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * terrian.GetSize() + sizeof(normal3) * terrian.GetSize(), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * terrian.GetSize(), terrian.GetPureTerrian().get());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * terrian.GetSize(), sizeof(normal3) * terrian.GetSize(), terrian.GetNormals().get());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrian.GetIndicesNum() * sizeof(GLuint), terrian.GetIndices().get(), GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(point4) * terrian.GetSize()));

	glBindVertexArray(0); // Unbind VAO

	char bGameLoopRunning = 1;

	int frame = 0;
	double time = 0;


	while (bGameLoopRunning)
	{
		timer.Start();

		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				bGameLoopRunning = 0;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				bGameLoopRunning = 0;
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w){
				camera.Move(FORWARD, (float)timer.GetElapsedTime());
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
				camera.Move(BACKWARD, (float)timer.GetElapsedTime());
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
				camera.Move(LEFT, (float)timer.GetElapsedTime());
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
				camera.Move(RIGHT, (float)timer.GetElapsedTime());
			}
			else if (e.type == SDL_MOUSEMOTION){
				CameraMotion((float)e.motion.x, (float)e.motion.y, &camera);
			}
		}



		// Draw Stuff: Render()
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sh.Use();
		camera.Update();

		/* drawing code in here! */
		// Create camera transformation
		glm::mat4 view;
		view = *(camera.GetViewMatrix());
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
		// Get the uniform locations
		//GLint modelLoc = glGetUniformLocation(sh.GetID(), "model");
		GLint viewLoc = glGetUniformLocation(sh.GetID(), "view");
		GLint projLoc = glGetUniformLocation(sh.GetID(), "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, terrian.GetIndicesNum(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		window.SwapBuffers();


		time += timer.GetElapsedTime();
		timer.Stop();
		++frame;
		if (time / 1000 >= 1 && time < 60){
			std::cout << "Frame rate warning:(lower than 60 fps) "<< frame << std::endl;
			frame = 0;
			time = 0;
		}
	}

	window.DestoryGL();
	SDL_Quit();
	return 0;
}