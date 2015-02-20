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
GLfloat lastX = 512, lastY = 384;
bool firstMouse = true;

Shader sh;
MyTimer timer;
FreeCamera camera(point3(-1.0f, 0.0f, 1.5f));
Terrian terrian;

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
	//GLuint VAO, VBO, IBO;
	terrian.GenHightMap("Assets/Terrian/height_map.jpg");

	char bGameLoopRunning = 1;
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
				//CameraMotion(e.motion.x, e.motion.y, &camera);
			}
		}



		// Draw Stuff: Render()
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sh.Use();
		camera.Update();




		window.SwapBuffers();
		timer.Stop();

		//std::cout << "FPS: "<< 1000 / (timer.GetElapsedTime())  << std::endl;
	}

	window.DestoryGL();
	SDL_Quit();
	return 0;
}