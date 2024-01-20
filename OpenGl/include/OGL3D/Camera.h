#pragma once
#include "OGL3D/MVP.h"
#include <glm/glm.hpp>
#include <Windows.h>

class Camera {
private:
	glm::vec2 angle;
	float sensetivity;
	MVP* mvp;
	void RotateCamera(float xAngle, float zAngle);

public:
	Camera(MVP* mvp, float sens) : mvp(mvp), sensetivity(sens) { angle = glm::vec2(0, 0); };
	void MoveCamera(double delta_time);
	glm::vec2 GetAngle() const;
};
