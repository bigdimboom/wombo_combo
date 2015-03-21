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
#include "Octree.h"
#include "TerrainRenderable.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Put mouse in the center
GLfloat lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Window gWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedural Terrian");

Shader gShader;
Shader gShaderOctree;
MyTimer gTimer;
FreeCamera gCamera(point3(100.0f, 25.0f, 50.5f));

point3 gLightPos = point3(-1.0, 100, 1.5);

bool gIsGameLoopRunning = false;
int gFrameCount = 0;
double gTimeElapsed = 0.0;

Octree octree;

TerrainRenderable gTerrain;


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
	gShaderOctree.Comiple("Shaders/vs_octree.glsl", "Shaders/fs_octree.glsl");

	gTimer.Reset();

	gTerrain.AttachTexture("Assets/Terrain/terrain_tex.jpg", "grass_texture");
	gTerrain.AttachTexture("Assets/Terrain/dirt.JPG", "dirt_texture");
	gTerrain.AttachTexture("Assets/Terrain/Rock.jpg", "rock_texture");
	gTerrain.AttachTexture("Assets/Terrain/snow.JPG", "snow_texture");
	gTerrain.AttachTexture("Assets/Terrain/heightmap.jpeg", "alpha_texture");
	gTerrain.Init();

	//octree.BindMesh(&gTerrain.GetRawTerrain()->GetMesh(), point3(0.0f, 0.0f, 0.0f), 512.0f / 2.0f);
	//octree.Build(600, 7);
}

void CullTest()
{

}

void EventHandler(SDL_Event &e)
{
	if (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			gIsGameLoopRunning = 0;
		else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
			gIsGameLoopRunning = 0;
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w){
			gCamera.Move(FORWARD, (float)gTimer.GetElapsedTime());
			CullTest();
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
			gCamera.Move(BACKWARD, (float)gTimer.GetElapsedTime());
			CullTest();
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
			gCamera.Move(LEFT, (float)gTimer.GetElapsedTime());
			CullTest();
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
			gCamera.Move(RIGHT, (float)gTimer.GetElapsedTime());
			CullTest();
		}
		else if (e.type == SDL_MOUSEMOTION){
			CameraMotion((float)e.motion.x, (float)e.motion.y, &gWindow, &gCamera);
			CullTest();
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

	//octree.DebugDraw(&gCamera, &gShaderOctree);

	gWindow.SwapBuffers();
}

void Update()
{
	gTimeElapsed += gTimer.GetElapsedTime();
	++gFrameCount;
	if (gTimeElapsed / 1000 >= 1/* && gTimeElapsed < 60*/){
		//std::cout << "Warning:(Frame rate lower than 60 fps) " << gFrameCount << std::endl;
		gFrameCount = 0;
		gTimeElapsed = 0;
		gLightPos.x += gLightPos.x * cos(1.0f) - gLightPos.y * sin(1.0f);
		gLightPos.y += gLightPos.x * sin(1.0f) - gLightPos.y * cos(1.0f);
	}
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return 1;
	}

	gWindow.Open();
	gWindow.InitGL();

	Init();
	gCamera.SetVelocity(2.0f);
	gCamera.SetFrustum(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

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

	gWindow.DestoryGL();
	SDL_Quit();
	return 0;
}