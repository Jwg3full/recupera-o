#version 410

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_mapping;

out vec2 texture_coords;

// IMPORTANTE: O nome deve ser exatamente "transformacao", tudo minúsculo e sem acento
uniform mat4 transformacao; 

void main () {
    texture_coords = texture_mapping;
    
    gl_Position = transformacao * vec4(vertex_position, 0.0, 1.0);
}