#pragma once

class IGameObject
{
public:
	virtual ~IGameObject(){}
	virtual void Update() = 0;
};