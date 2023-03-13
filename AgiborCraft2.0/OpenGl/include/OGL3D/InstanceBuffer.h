#pragma once
#include "GLEW/glew.h"
#include "glm/glm.hpp"
#include <vector>

class InstanceBuffer {
public:
	InstanceBuffer();
	~InstanceBuffer();
	void addInstance(glm::mat4 model);
	void bind();
	void unbind();
	void SetInstacesBuffer(std::vector<glm::mat4>* instances);
	int NumOfInstances();
private:
	std::vector<glm::mat4>* m_instances;
	unsigned int gl_id;
};