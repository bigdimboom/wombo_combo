#pragma once

#include <Windows.h>
#include <Winbase.h>

class MyTimer
{
public:
	MyTimer();
	~MyTimer();
	void Reset();
	void Start();
	void Stop();
	// compute and print the elapsed time in millisec
	inline double GetElapsedTime() const { return _elapsedTime; }
private:
	LARGE_INTEGER _frequency;        // ticks per second
	LARGE_INTEGER _t1, _t2;           // ticks
	double _elapsedTime;
};

