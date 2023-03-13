#include "OGL3D/Shader.h"


u_int Shader::CompileShader(u_int type, const std::string& data) {
	u_int shader = glCreateShader(type);
	const char* source = &data[0];
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	int shader_status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
	if (shader_status == GL_FALSE) {
		printf("Error compiling %s shader:\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		int n;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &n);
		char* msg = (char*)malloc(sizeof(char) * n);
		glGetShaderInfoLog(shader, n, &n, msg);
		printf("%s\n", msg);
		free(msg);
		return 0;
	}
	return shader;
}


int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	u_int program = glCreateProgram();
	u_int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	u_int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}


GLint Shader::GetShaderUniformLocation(const std::string& name)
{
	auto locationSearch = shader_uniforms.find(name);
	if (locationSearch != shader_uniforms.end())
		return locationSearch->second; // locationSearch->first is the key (name) and locationSearch->second is the value
	GLint location = glGetUniformLocation(this->gl_id, name.c_str());
	shader_uniforms[name] = location;
	return location;
}

Shader::~Shader() {}

Shader::Shader(const std::string& filepath) {
	std::ifstream stream(filepath);
	if (stream.fail()) {
		std::cout << "coudn't open shader file!" << std::endl;
	}
	std::string line;
	std::stringstream ss[2];
	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	this->ver = ss[0].str();
	this->frag = ss[1].str();
	this->gl_id = CreateShader(this->ver, this->frag);
}

void Shader::bind() {
	glUseProgram(this->gl_id);
}
void Shader::unbind() {
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	int location = GetShaderUniformLocation(name);
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	int location = GetShaderUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
	int location = GetShaderUniformLocation(name);
	glUniform3f(location, v0, v1, v2);
}
void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
	int location = GetShaderUniformLocation(name);
	glUniform2f(location, v0, v1);
}

void Shader::SetUniformf(const std::string& name, float v0) {
	int location = GetShaderUniformLocation(name);
	glUniform1f(location, v0);
}

void Shader::SetUniformi(const std::string& name, int v0)
{
	int location = GetShaderUniformLocation(name);
	glUniform1i(location, v0);
}




