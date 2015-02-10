
#include <SDL.h>
#include <GL\glew.h>

int main(int argc, char** argv){

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_SHOWN);

	SDL_Event ev;
	bool isRunning = true;
	while (isRunning){
		SDL_PollEvent(&ev);
		if (ev.type == SDL_QUIT){
			isRunning = false;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}