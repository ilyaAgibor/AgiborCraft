#include "OGL3D/Shader.h"


u_int Shader::CompileShader(u_int type, const std::string& data) {
	// Create a shader object of the specified type
	u_int shader = glCreateShader(type);
	// Get a pointer to the data string
	const char* source = &data[0];
	// Set the shader source code
	glShaderSource(shader, 1, &source, nullptr);
	// Compile the shader
	glCompileShader(shader);
	// Check the compilation status of the shader
	int shader_status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
	if (shader_status == GL_FALSE) {
		// If compilation failed, print an error message and retrieve the shader info log
		printf("Error compiling %s shader:\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		// Get the length of the shader info log
		int log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		// Allocate memory for the info log
		char* log_message = (char*)malloc(sizeof(char) * log_length);
		// Retrieve the shader info log
		glGetShaderInfoLog(shader, log_length, &log_length, log_message);
		// Print the shader info log
		printf("%s\n", log_message);
		// Free the allocated memory
		free(log_message);
		// Return 0 to indicate an error
		return 0;
	}
	// Return the ID of the compiled shader
	return shader;
}


// Create a shader program and return its ID
int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// Create a shader program object
	u_int program = glCreateProgram();
	// Compile the vertex shader and get its ID
	u_int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// Compile the fragment shader and get its ID
	u_int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	// Link the shader program
	glLinkProgram(program);
	// Validate the shader program
	glValidateProgram(program);
	// Delete the vertex and fragment shaders (no longer needed after linking)
	glDeleteShader(vs);
	glDeleteShader(fs);
	// Return the ID of the created shader program
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




