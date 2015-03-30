#pragma once
class IMovable
{
public:
	virtual ~IMovable() {}
	virtual void Move(float dTime) = 0;
};

