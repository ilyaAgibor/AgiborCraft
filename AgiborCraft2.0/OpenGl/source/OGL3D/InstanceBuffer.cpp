#include "OGL3D/InstanceBuffer.h"

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &gl_id);
    glBindBuffer(GL_ARRAY_BUFFER, gl_id);

	int m_instanceattribute = 3;
    glEnableVertexAttribArray(m_instanceattribute);
    glEnableVertexAttribArray(m_instanceattribute + 1);
    glEnableVertexAttribArray(m_instanceattribute + 2);
    glEnableVertexAttribArray(m_instanceattribute + 3);

    glVertexAttribPointer(m_instanceattribute, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glVertexAttribPointer(m_instanceattribute + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glVertexAttribPointer(m_instanceattribute + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribPointer(m_instanceattribute + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(m_instanceattribute, 1);
    glVertexAttribDivisor(m_instanceattribute + 1, 1);
    glVertexAttribDivisor(m_instanceattribute + 2, 1);
    glVertexAttribDivisor(m_instanceattribute + 3, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

InstanceBuffer::~InstanceBuffer() {
	glDeleteBuffers(1, &this->gl_id);
}

void InstanceBuffer::addInstance(glm::mat4 model) {
	m_instances->push_back(model);
}



void InstanceBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instances->size(), &m_instances->data()[0], GL_DYNAMIC_DRAW);
}

void InstanceBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceBuffer::SetInstacesBuffer(std::vector<glm::mat4>* instances)
{
    this->m_instances = instances;
}

int InstanceBuffer::NumOfInstances()
{
    return this->m_instances->size();
}
