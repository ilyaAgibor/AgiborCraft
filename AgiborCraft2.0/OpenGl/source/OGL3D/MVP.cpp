#include "OGL3D/MVP.h"

MVP::MVP(float fov)
{
    this->fov = fov;
    this->proj = glm::perspective(glm::radians(fov), 1200.0f / 800.0f, 0.1f, 100.0f);
    this->model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    this->_mvp = this->proj * this->model;
}

MVP::~MVP()
{

}

void MVP::Translate(glm::vec3 units)
{
    this->model = glm::translate(this->model, units);
    this->_mvp = this->proj * this->model;
}

void MVP::Rotate(float a, glm::vec3 axis)
{
    this->model = glm::rotate(this->model, a, axis);
    this->_mvp = this->proj * this->model;
}

void MVP::Scale(glm::vec3 units)
{

}

void MVP::PushMatrix()
{
    this->mat_vec.push_back(this->_mvp);
    this->mat_vec.push_back(this->model);
    this->top_mat_vec+=2;
}

void MVP::PopMatrix()
{
    this->model = this->mat_vec[this->top_mat_vec-1];
    this->mat_vec.pop_back();
    this->_mvp = this->mat_vec[this->top_mat_vec-1];
    this->mat_vec.pop_back();
    this->top_mat_vec -= 2;

}

void MVP::ChangePrespectiveRatio(float ratio)
{
    this->proj = glm::perspective(glm::radians(this->fov), ratio, 0.1f, 100.0f);
}

glm::mat4 MVP::GetMvp()
{
    return this->_mvp;
}
