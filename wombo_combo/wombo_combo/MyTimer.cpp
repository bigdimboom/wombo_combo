#include "MyTimer.h"


MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&_frequency);
	_elapsedTime = 0.0;
}


MyTimer::~MyTimer()
{
}

void MyTimer::Start()
{
	QueryPerformanceCounter(&_t1);
}

void MyTimer::Stop()
{
	QueryPerformanceCounter(&_t2);

	_elapsedTime = (_t2.QuadPart - _t1.QuadPart) * 1000.0 / _frequency.QuadPart;
}

void MyTimer::Reset()
{
	_elapsedTime = 0.0;
}