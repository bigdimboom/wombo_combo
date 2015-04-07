#include "Camera.h"


Camera::Camera(point3 cameraPos, point3 wolrdUp)
	:Actor(cameraPos),
	_up(point3(0.0f, 1.0f, 0.0f)),
	_worldUp(_up),
	_front(point3(0.0f, 0.0f, 1.0f)),
	_sensitivity(0.5f),
	_velocity(0.05f),
	_projMat(1.0),
	_isProjSet(false)
{
	Update();
}


Camera::~Camera()
{
}

void Camera::Reset()
{
	position = point3(0.0f, 0.0f, 3.0f);
	_up = point3(0.0f, 1.0f, 0.0f);
	_front = point3(0.0f, 0.0f, 1.0f);
	_viewMat = glm::lookAt(position, position + _front, _up);
	_right = glm::normalize(glm::cross(_front, _up));
}

void Camera::Move(Camera_Movement dir, GLfloat dTime)
{

	GLfloat dist = dTime * _velocity;

	switch (dir){
	case FORWARD:
		position += _front * dist;
		break;
	case BACKWARD:
		position -= _front * dist;
		break;
	case LEFT:
		position -= _right * dist;
		break;
	case RIGHT:
		position += _right * dist;
		break;
	}
}

void Camera::Rotate(Camera_Rotate dir, GLfloat angle)
{
	angle *= _sensitivity;

	switch (dir){
	case PITCH:
		_pitch += angle;
		break;
	case YAW:
		_yaw += angle;
		break;
	case ROLL:
		_roll += angle;
		break;
	}
}

void Camera::Update()
{
	// Calculate the new Front vector
	point3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	// Also re-calculate the Right and Up vector

	//Why world Up
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
	// Update ViewMat
	_viewMat = glm::lookAt(position, position + _front, _up);
}

void Camera::SetFrustum(float angle, float ratio, float nearPlane, float farPlane)
{
	_projMat = glm::perspective(angle, ratio, nearPlane, farPlane);
	_fov = angle;
	_ratio = ratio;
	_nearPlane = nearPlane;
	_farPlane = farPlane;
	_isProjSet = true;
}