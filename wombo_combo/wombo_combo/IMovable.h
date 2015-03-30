#pragma once
class IMovable
{
public:
	~IMovable();
	virtual void Move(float dTime) = 0;
};

