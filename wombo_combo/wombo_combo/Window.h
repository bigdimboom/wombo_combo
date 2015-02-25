#pragma once

#include "Global.h"

class Window
{
public:
	Window(int width, int height, const char* title);
	virtual ~Window();
	bool Open();
	void Close();
	bool InitGL();
	void DestoryGL();
	void SetGLContext();

	void EnableVSync(){
		_isVSyncOn = true;
		SDL_GL_SetSwapInterval(1);
	}
	void DisableEnableVSync(){
		_isVSyncOn = false;
		SDL_GL_SetSwapInterval(0);
	}
	inline bool IsVSyncOn() const{ return _isVSyncOn;}
	inline bool IsClosed() const{ return _isClose; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }
	inline void SetTitle(std::string str) const{ SDL_SetWindowTitle(_pWindow, str.c_str()); }
	inline SDL_Window* GetWindowRef() const { return _pWindow; }
	void SwapBuffers();
private:
	SDL_Window* _pWindow;
	std::string _title;
	int _width;
	int _height;
	bool _isClose;

	//Opengl Context
	SDL_GLContext _glContext;
	bool _isVSyncOn;
private:
	Window(const Window& other) {}
	void operator=(const Window& other) {}
};

