#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stack>
#include <vector>

class MVP {
public:
	MVP(float fov);
	~MVP();
	void Translate(glm::vec3 units);
	void Rotate(float a, glm::vec3 axis);
	void Scale(glm::vec3 units);
	void PushMatrix();
	void PopMatrix();
	void ChangePrespectiveRatio(float ratio);
	glm::mat4 GetMvp();

private:
	unsigned int top_mat_vec = 0;
	std::vector<glm::mat4> mat_vec;
	glm::mat4 proj;
	glm::mat4 model;
	glm::mat4 _mvp;
	float fov;
	
};