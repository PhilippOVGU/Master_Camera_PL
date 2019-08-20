#pragma once
#include "glm/glm.hpp" //mathe für bewegungen
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
public:
	Camera(float fov,float width,float hight) {

		projection = glm::perspective(fov / 2.0f, width / hight, 0.1f, 100.0f);
		view = glm::mat4(1.0f);
		position = glm::vec3(0.0f);
		update();
	};
	//~Camera();
	glm::mat4 getView() {
		return view;
	}
	glm::mat4 getViewProj() {
		return viewProj;
	}
	virtual void update() {
		viewProj = projection * view;
	}
	virtual void translate(glm::vec3 v) {
		position = position + v;
		view = glm::translate(view, v * -1.0f);
	}
protected:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;
};

