#pragma once
#include "camera.h"

class FPSCamera:public Camera
{
public:
	FPSCamera(float fov, float width, float hight):Camera(fov,width,hight) {
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		onMouseMoved(0.0f, 0.0f);
		update();
	};

	void onMouseMoved(float xRel, float yRel) {
		yaw = yaw + xRel*mouseSensitivity;
		pitch = pitch + yRel * mouseSensitivity;
		if (pitch > 89.0f) {
			pitch = 89.0f;
		};
		if (pitch<-89.0f)
		{
			pitch = -89.0f;
		}
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		lookAt = glm::normalize(front);
		update();
	};

	void update() override {
		view = glm::lookAt(position, position + lookAt, up);
		//view = glm::lookAt(position, lookAt, up);
		viewProj = projection * view;
	}
	void updatecenter(glm::vec3 v)  {
		lookAt = v;
		
		update();
	}
	void setposition(glm::vec3 v) {
		position = v;
	}
	void moveFront(float amount){
		translate(glm::normalize(glm::vec3(1.0f,0.0f,1.0f)*lookAt) * amount);
		update();
	}
	void moveSidways(float amount) {
		translate(glm::normalize(cross(lookAt, up)) * amount);
		update();
	}
	
protected:
	float yaw;
	float pitch;
	glm::vec3 lookAt;
	const float mouseSensitivity = 0.3f;
	glm::vec3 up;

};
