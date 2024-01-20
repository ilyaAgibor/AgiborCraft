#shader vertex
#version 330 core

layout(location = 8) in vec4 position;
//layout(location = 1) in mat4 mvp;

uniform mat4 u_MVP;
uniform mat4 pos;

void main() {
	gl_Position = u_MVP * pos * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(0.8f, 0.1f, 0.7f, 1.0f);
}