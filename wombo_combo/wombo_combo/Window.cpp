#include "Window.h"

Window::Window(int width, int height, const char* title)
	:_title(title), _width(width), _height(height),
	_isClose(true), _pWindow(nullptr), _isVSyncOn(false)
{

}


Window::~Window()
{
}

bool Window::Open()
{
	// Create an application window with the following settings:
	_pWindow = SDL_CreateWindow(
		_title.c_str(),                  // window title
		SDL_WINDOWPOS_UNDEFINED,         // initial x position
		SDL_WINDOWPOS_UNDEFINED,         // initial y position
		_width,                    // width, in pixels
		_height,                   // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		// flags - see below
		);

	// Check that the window was successfully made
	if (_pWindow == nullptr) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void Window::Close()
{
	_isClose = true;
	SDL_DestroyWindow(_pWindow);
}

bool Window::InitGL()
{
	assert(_pWindow != nullptr);
	_glContext = SDL_GL_CreateContext(_pWindow);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	glewExperimental = GL_TRUE;

	GLenum glew_status = glewInit();

	if (glew_status != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(glew_status));
		return false;
	}

	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	if (_isVSyncOn)
	{
		SDL_GL_SetSwapInterval(1);
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	// Define the viewport dimensions
	glViewport(0, 0, _width, _height);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Window::DestoryGL()
{
	assert(_glContext != nullptr);
	SDL_GL_DeleteContext(_glContext);
}

void Window::SwapBuffers()
{
	assert(_glContext != nullptr);
	SDL_GL_SwapWindow(_pWindow);
}

void Window::SetGLContext()
{
	assert(_pWindow != nullptr && _glContext != nullptr);
	SDL_GL_MakeCurrent(_pWindow, _glContext);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GetWidth(), GetHeight());
}
