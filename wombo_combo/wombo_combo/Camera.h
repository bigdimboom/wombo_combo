#pragma once
#include "Global.h"
#include "Actor.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Rotate {
	PITCH,
	YAW,
	ROLL
};

class Camera  : public Actor
{
public:
	Camera(point3 cameraPos = point3(0.0f, 0.0f, 3.0f), 
		point3 worldUp = point3(0.0f, 1.0f, 0.0f) );
	~Camera();
	void Reset();
	inline matrix4* GetViewMatrix(){return &_viewMat; }
	void SetFrustum(float angle, float ratio, float nearPlane, float farPlane);
	inline matrix4* GetProjMatrix(){ return &_projMat; }
	virtual void Move(Camera_Movement dir, GLfloat dTime);
	virtual void Rotate(Camera_Rotate dir, GLfloat angle);
	inline void SetVelocity(GLfloat velocity) { _velocity = velocity; }
	inline void SetSensitivity(GLfloat sensitivity) { _sensitivity = sensitivity; }
	// If you want to override the camera class
	virtual void Update();
	//view matrix
protected:
	matrix4 _viewMat;
	matrix4 _projMat;
	//Camera Attributes
	point3 _up;
	point3 _right;
	point3 _front;
	//Other Options
	GLfloat _sensitivity;
	GLfloat _velocity;
	// Eular Angles
	GLfloat _yaw;
	GLfloat _pitch;
	GLfloat _roll;
	//World UP
	point3 _worldUp;
};

