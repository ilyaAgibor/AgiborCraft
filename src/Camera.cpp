#include "Camera.h"
#include <iostream>

void Camera::RotateCamera(float xAngle, float zAngle) {
    this->angle.x += xAngle;
    this->angle.y += zAngle;
    
	if (this->angle.y < 0) this->angle.y += 360;
	if (this->angle.y > 360) this->angle.y -= 360;
	if (this->angle.x <= 0) this->angle.x = 0;
	if (this->angle.x >= 180) this->angle.x = 180;
}


void Camera::MoveCamera(double delta_time) {
    POINT cur;
    static POINT base = { 500,500 };
    GetCursorPos(&cur);
    RotateCamera((base.y - cur.y) * this->sensetivity, (base.x - cur.x) * this->sensetivity);
    SetCursorPos(base.x, base.y);
    this->mvp->Rotate(-glm::radians(this->angle.x), glm::vec3(1, 0, 0));
    this->mvp->Rotate(-glm::radians(this->angle.y), glm::vec3(0, 0, 1));
}

glm::vec2 Camera::GetAngle() const
{
    return this->angle;
}

