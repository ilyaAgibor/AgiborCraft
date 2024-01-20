#shader vertex
#version 330 core

layout(location = 0) in vec4 position;              // Vertex position input
layout(location = 1) in float aLightIntensity;      // Light intensity input
layout(location = 2) in vec2 texCoord;              // Texture coordinates input
layout(location = 3) in mat4 model;                 // Model matrix input (takes 4 slots!)
layout(location = 7) in float index;                // Index input

uniform mat4 u_MVP;                                 // Model-View-Projection matrix uniform
uniform vec3 u_Selected;                            // Selected vector uniform

out float LightIntensity;                           // Output light intensity
out vec2 v_TexCoord;                                // Output texture coordinates
flat out int v_Index;                               // Flat output index
flat out int v_Selected;                            // Flat output selected flag

vec3 getPosition(mat4 model) {
    vec4 position = model[3];
    return vec3(position.x, position.y, position.z);
}

void main() {
    gl_Position = u_MVP * model * position;          // Calculate the final position of the vertex
    LightIntensity = aLightIntensity;                // Pass the light intensity to the fragment shader
    v_TexCoord = texCoord;                           // Pass the texture coordinates to the fragment shader
    v_Index = int(index);                            // Convert the index to an integer and pass it to the fragment shader

    if (getPosition(model) == u_Selected) {          // Check if the vertex position is equal to the selected vector
        v_Selected = 1;                              // Set the selected flag to 1 if true
    }
    else {
        v_Selected = 0;                              // Set the selected flag to 0 if false
    }
}



#shader fragment
#version 330 core

in vec2 v_TexCoord;                                 // Input texture coordinates
in float LightIntensity;                            // Input light intensity
flat in int v_Index;                                // Flat input index
flat in int v_Selected;                             // Flat input selected flag

layout(location = 0) out vec4 color;                // Output color

uniform sampler2DArray u_Texture;                   // Texture sampler

void main() {
    vec4 texColor = texture(u_Texture, vec3(v_TexCoord, v_Index));      // Sample the texture using the texture coordinates and index

    float borderThickness = 0.01f;                   // Thickness of the border

    // Calculate the final color based on the sampled texture color and light intensity
    color = vec4(texColor.x * LightIntensity, texColor.y * LightIntensity, texColor.z * LightIntensity, texColor.w);

    if (v_Selected == 1) {
        // Apply a border color if the vertex is selected and the texture coordinates are within the border thickness
        if (v_TexCoord.x < borderThickness || v_TexCoord.x > 1.0 - borderThickness ||
            v_TexCoord.y < borderThickness || v_TexCoord.y > 1.0 - borderThickness) {
            color = vec4(0, 0, 0, 1.0);              // Set the color to black
        }
    }
}