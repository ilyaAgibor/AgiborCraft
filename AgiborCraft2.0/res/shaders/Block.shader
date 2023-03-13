#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in float aLightIntensity;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in mat4 model;		//takes 4 slots!
layout(location = 7) in float index;

uniform mat4 u_MVP;
uniform vec3 u_Selected;

out float LightIntensity;
out vec2 v_TexCoord;
flat out int v_Index;
flat out int v_Selected;

vec3 getPosition(mat4 model) {
	vec4 position = model[3];
	return vec3(position.x, position.y, position.z);
}

void main() {
	gl_Position = u_MVP * model * position;
	LightIntensity = aLightIntensity;
	v_TexCoord = texCoord;
	v_Index = int(index);
	if (getPosition(model) == u_Selected) {
		v_Selected = 1;
	}
	else
		v_Selected = 0;
}



#shader fragment
#version 330 core

in vec2 v_TexCoord;
in float LightIntensity;
flat in int v_Index;
flat in int v_Selected;


layout(location = 0) out vec4 color;

uniform sampler2DArray u_Texture;

void main() {
	
	vec4 texColor = texture(u_Texture, vec3(v_TexCoord, v_Index));

	float borderThickness = 0.01f;

	color = vec4(texColor.x * LightIntensity, texColor.y * LightIntensity, texColor.z * LightIntensity, texColor.w);
	if (v_Selected == 1) {
		if (v_TexCoord.x < borderThickness || v_TexCoord.x > 1.0 - borderThickness ||
			v_TexCoord.y < borderThickness || v_TexCoord.y > 1.0 - borderThickness) {
			color = vec4(0,0,0,1.0);
		}
	}	
}