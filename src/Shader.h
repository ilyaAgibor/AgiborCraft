#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GLEW/glew.h>
#include <unordered_map>
#include "glm/glm.hpp"

typedef unsigned int u_int;

class Shader {
private:
	unsigned int gl_id;
	std::string ver;
	std::string frag;
	std::unordered_map<std::string, GLint> shader_uniforms;
	GLint GetShaderUniformLocation(const std::string& name);
	static u_int CompileShader(u_int type, const std::string& data);
	static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
public:
	Shader(const std::string& filepath);
	~Shader();
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniformf(const std::string& name, float v0);
	void SetUniformi(const std::string& name, int v0);
	void bind();
	void unbind();
};
