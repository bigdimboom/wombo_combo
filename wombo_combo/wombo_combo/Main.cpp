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
#include "Terrain.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Put mouse in the center
GLfloat lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Window gWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Procedural Terrian");

Shader gShader;
Shader gShader2;
MyTimer gTimer;
FreeCamera gCamera(point3(127.0f, 100.0f, 143.5f));
Terrain gTerrain(1024, 1024);

Texture gTerrainTex_alpha;
Texture gTerrainTex_grass;
Texture gTerrainTex_dirt;
Texture gTerrainTex_rock;
Texture gTerrainTex_snow;

point3 gLightPos = point3(-1.0, 100, 1.5);
GLuint gVAO, gVBO, gEBO;
GLuint gVAO2, gVBO2, gEBO2;

bool gIsGameLoopRunning = false;
int gFrameCount = 0;
double gTimeElapsed = 0.0;

GLfloat octree[] = {
	-1024.0f / 2, 1024.0f / 2, 1024.0f / 2, //0
	1024.0f / 2, 1024.0f / 2, 1024.0f / 2, //1
	1024.0f / 2, -1024.0f / 2, 1024.0f / 2, //2
	-1024.0f / 2, -1024.0f / 2, 1024.0f / 2, //3
	-1024.0f / 2, 1024.0f / 2, -1024.0f / 2, //0
	1024.0f / 2, 1024.0f / 2, -1024.0f / 2, //1
	1024.0f / 2, -1024.0f, -1024.0f / 2, //2
	-1024.0f / 2, -1024.0f / 2, -1024.0f / 2, //3
};

GLuint lines[] =
{
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	4, 5,
	5, 6,
	6, 7,
	7, 4,
	0, 4,
	3, 7,
	1, 5,
	2, 6
};


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
	gShader2.Comiple("Shaders/vs_octree.glsl", "Shaders/fs_octree.glsl");

	gTimer.Reset();

	gShader.Use();
	gTerrain.GenTerrian("Assets/Terrain/height_map2.jpg", true, true);

	gTerrainTex_alpha.Load("Assets/Terrain/height_map2.jpg", true);
	gTerrainTex_grass.Load("Assets/Terrain/terrain_tex.jpg", true);
	gTerrainTex_dirt.Load("Assets/Terrain/dirt.JPG", true);
	gTerrainTex_rock.Load("Assets/Terrain/Rock.jpg", true);
	gTerrainTex_snow.Load("Assets/Terrain/snow.JPG", true);

	glGenVertexArrays(1, &gVAO);
	glGenBuffers(1, &gVBO);
	glGenBuffers(1, &gEBO);

	glBindVertexArray(gVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * gTerrain.GetVertsSize() +
		sizeof(normal3) * gTerrain.GetVertsSize() +
		sizeof(point2) * gTerrain.GetVertsSize(), nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * gTerrain.GetVertsSize(), gTerrain.GetPureTerrian().get());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * gTerrain.GetVertsSize(), sizeof(normal3) * gTerrain.GetVertsSize(), gTerrain.GetNormals().get());

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * gTerrain.GetVertsSize() +
		sizeof(normal3) * gTerrain.GetVertsSize(), sizeof(point2) * gTerrain.GetVertsSize(), gTerrain.GetTextureCoords().get());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gTerrain.GetIndicesSize() * sizeof(GLuint), gTerrain.GetIndices().get(), GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(point4) * gTerrain.GetVertsSize()));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(point4) * gTerrain.GetVertsSize() + sizeof(normal3) * gTerrain.GetVertsSize()));

	glBindVertexArray(0); // Unbind VAO

	gShader2.Use();
	glGenVertexArrays(1, &gVAO2);
	glGenBuffers(1, &gVBO2);
	glGenBuffers(1, &gEBO2);

	glBindVertexArray(gVAO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(octree), octree, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0); // Unbind VAO

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
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s){
			gCamera.Move(BACKWARD, (float)gTimer.GetElapsedTime());
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a){
			gCamera.Move(LEFT, (float)gTimer.GetElapsedTime());
		}
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d){
			gCamera.Move(RIGHT, (float)gTimer.GetElapsedTime());
		}
		else if (e.type == SDL_MOUSEMOTION){
			CameraMotion((float)e.motion.x, (float)e.motion.y, &gWindow, &gCamera);
		}
	}
}

void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gShader.Use();
	gCamera.Update();

	/* drawing code in here! */
	// Create camera transformation
	glm::mat4 view;
	view = *(gCamera.GetViewMatrix());
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(gShader.GetID(), "model");
	GLint viewLoc = glGetUniformLocation(gShader.GetID(), "view");
	GLint projLoc = glGetUniformLocation(gShader.GetID(), "projection");

	GLint lightPosLoc = glGetUniformLocation(gShader.GetID(), "lightPosition");
	glUniform3f(lightPosLoc, gLightPos.x, gLightPos.y, gLightPos.z);
	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix4(1.0f)));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(gVAO);

	gTerrainTex_grass.Bind(0);
	glUniform1i(glGetUniformLocation(gShader.GetID(), "grass_texture"), 0);
	gTerrainTex_dirt.Bind(1);
	glUniform1i(glGetUniformLocation(gShader.GetID(), "dirt_texture"), 1);
	gTerrainTex_rock.Bind(2);
	glUniform1i(glGetUniformLocation(gShader.GetID(), "rock_texture"), 2);
	gTerrainTex_snow.Bind(3);
	glUniform1i(glGetUniformLocation(gShader.GetID(), "snow_texture"), 3);

	gTerrainTex_alpha.Bind(4);
	glUniform1i(glGetUniformLocation(gShader.GetID(), "alpha_texture"), 4);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, gTerrain.GetIndicesSize(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);


	gShader2.Use();
	glBindVertexArray(gVAO2);
	// Get the uniform locations
	modelLoc = glGetUniformLocation(gShader2.GetID(), "model");
	viewLoc = glGetUniformLocation(gShader2.GetID(), "view");
	projLoc = glGetUniformLocation(gShader2.GetID(), "projection");

	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matrix4(1.0f)));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glDrawElements(GL_LINES, sizeof(lines), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);

	gWindow.SwapBuffers();
}

void Update()
{
	gTimeElapsed += gTimer.GetElapsedTime();
	++gFrameCount;
	if (gTimeElapsed / 1000 >= 1 && gTimeElapsed < 60){
		std::cout << "Warning:(Frame rate lower than 60 fps) " << gFrameCount << std::endl;
		gFrameCount = 0;
		gTimeElapsed = 0;
		//std::cout << camera.GetPosition()->x << camera.GetPosition()->y << camera.GetPosition()->z << std::endl;
		gLightPos.x += gLightPos.x * cos(5.0f) - gLightPos.y * sin(5.0f);
		gLightPos.y += gLightPos.x * sin(5.0f) - gLightPos.y * cos(5.0f);
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
	gCamera.SetVelocity(0.8f);

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