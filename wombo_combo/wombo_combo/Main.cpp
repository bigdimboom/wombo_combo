#define NOMINMAX

#include "Global.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "MyTimer.h"
#include "FreeCamera.h"
#include "Octree.h"
#include "TerrainRenderable.h"
#include "FlockRenderable.h"
#include "Sphere.h"
#include "MeshRender.h"
#include "Triangle.h"
#include "Line.h"
#include <time.h>
#include "DebugDrawManager.h"
#include "SimpleCDTest.h"
#include "ParticleEffect.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MOUSE_TIME_CLIP 0.5

//Put mouse in the center
GLfloat lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Window gWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedural Terrian");

Shader gShader;
Shader gShaderDebug;
Shader gShaderParticle;

MyTimer gTimer;

FreeCamera gCamera(point3(10.0f, 25.0f, 10.5f));

point3 gLightPos = point3(-1.0, 100, 1.5);

bool gIsGameLoopRunning = false;
int gFrameCount = 0;
double gTimeElapsed = 0.0;

TerrainRenderable gTerrain(512, 512);
FlockRenderable gFlock;

bool gIsCulling = true;
bool gIsDebug = false;

SimpleCDTest gCDTest(&gTerrain);

ParticleEffect gPEffect;


void CameraMotion(GLfloat xpos, GLfloat ypos, Window* win, FreeCamera* cam){
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		SDL_ShowCursor(SDL_DISABLE);
		//SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_WarpMouseInWindow(win->GetWindowRef(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	cam->Rotate(PITCH, yoffset);
	cam->Rotate(YAW, xoffset);

	if (xpos == WINDOW_WIDTH - 1 || xpos == 0)
	{
		SDL_WarpMouseInWindow(win->GetWindowRef(), (int)WINDOW_WIDTH / 2, (int)lastY);
		lastX = WINDOW_WIDTH / 2;
	}

	if (ypos == WINDOW_HEIGHT - 1 || ypos == 0)
	{
		SDL_WarpMouseInWindow(win->GetWindowRef(), (int)lastX, (int)lastY);
	}
}

void Init()
{
	gShader.Comiple("Shaders/vs.glsl", "Shaders/fs.glsl");
	gShaderDebug.Comiple("Shaders/vs_debug.glsl", "Shaders/fs_debug.glsl");
	gShaderParticle.Comiple("Shaders/vs_particle.glsl", "Shaders/fs_particle.glsl");

	gTimer.Reset();

	gTerrain.AttachTexture("Assets/Terrain/terrain_tex.jpg", "grass_texture");
	gTerrain.AttachTexture("Assets/Terrain/dirt.JPG", "dirt_texture");
	gTerrain.AttachTexture("Assets/Terrain/Rock.jpg", "rock_texture");
	gTerrain.AttachTexture("Assets/Terrain/snow.JPG", "snow_texture");
	gTerrain.AttachTexture("Assets/Terrain/heightmap.jpeg", "alpha_texture");
	gTerrain.Init();

	//gFlock.Init();

	gPEffect.Init();

	//DebugDrawManager::getInstance().EnableWorldPlane(point4(0.2f, 0.6f, 0.4f, 1.0f), 1000.0f, 512, 1.0f, false);
}

void EventHandler(SDL_Event &e)
{
	if (SDL_PollEvent(&e))
	{
		if (gIsCulling){ gTerrain.EnableCull(true); }
		
		if (e.type == SDL_QUIT)
			gIsGameLoopRunning = 0;
		else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
			gIsGameLoopRunning = 0;
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_w:
				gCamera.Move(FORWARD, MOUSE_TIME_CLIP);
				break;
			case SDLK_s:
				gCamera.Move(BACKWARD, MOUSE_TIME_CLIP);
				break;
			case SDLK_a:
				gCamera.Move(LEFT, MOUSE_TIME_CLIP);
				break;
			case SDLK_d:
				gCamera.Move(RIGHT, MOUSE_TIME_CLIP);
				break;
			case SDLK_c:
				gIsCulling = !gIsCulling;
				break;
			default:
				break;
			}
			//std::cout << glm::to_string(gCamera.GetPosition()) << std::endl;
		}
		else if (e.type == SDL_MOUSEMOTION){
			CameraMotion((float)e.motion.x, (float)e.motion.y, &gWindow, &gCamera);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			/* If the left button was pressed. */
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				//std::cout << "Clicked\n";
				//gCDTest.SetMovingObj(gCamera.GetPosition(), gCamera.GetFront());
			}
		}
	}
}

void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gShader.Use();
	gCamera.Update();

	gTerrain.Render(&gCamera, &gLightPos, &gShader);

	//gFlock.Render(&gCamera, nullptr, &gShaderFlock);
	if (!gIsCulling)
	{
		if (!gIsDebug)
		{
			gTerrain.EnableDebug(true);
			gIsDebug = true;
		}

		gTerrain._frustum.Set(&gCamera, true);
		gTerrain._octree.DebugDraw(&gCamera, &gShaderDebug, color4(1.0, 0.0f, 0.0f, 1.0f));
	}


	gPEffect.Render(&gShaderParticle, &gCamera);

	DebugDrawManager::getInstance().Render(&gCamera, &gShaderDebug);

	gWindow.SwapBuffers();
}

void Update()
{
	gTimeElapsed += gTimer.GetElapsedTime();

	if (gTimeElapsed / 1000 >= 0.1f){
		//gFlock.MoveAll(1.0f);
		gCDTest.Update(0.1f);
		//gPEffect.Update(0.1f);
	}

	++gFrameCount;
	if (gTimeElapsed / 1000 >= 1/* && gTimeElapsed < 60*/){
		//std::cout << "Warning:(Frame rate lower than 60 fps) " << gFrameCount << std::endl;
		SDL_SetWindowTitle(gWindow.GetWindowRef(), std::to_string(gFrameCount / 2).c_str());
		gFrameCount = 0;
		gTimeElapsed = 0;
		gLightPos.x += gLightPos.x * cos(1.0f) - gLightPos.y * sin(1.0f);
		gLightPos.y += gLightPos.x * sin(1.0f) - gLightPos.y * cos(1.0f);
	}
}

void CleanUp()
{

}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Quit();
		return 1;
	}

	gWindow.Open();
	gWindow.InitGL();
	srand((unsigned int)time(NULL));

	Init();
	gCamera.SetVelocity(2.8f);
	gCamera.SetFrustum(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);

	SDL_Event e;

	//let the game begin
	gIsGameLoopRunning = true;

	while (gIsGameLoopRunning)
	{
		gTimer.Start();
		EventHandler(e);
		Render();
		Update();
		gTimer.Stop();
	}
	CleanUp();
	gWindow.DestoryGL();
	SDL_Quit();
	return 0;
}