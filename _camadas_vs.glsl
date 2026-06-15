#version 410

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_mapping;

out vec2 texture_coords;
uniform float layer_z;

void main () {
    // Inverte o eixo Y: 1.0 - y
    texture_coords = vec2(texture_mapping.x, 2 - texture_mapping.y);
    gl_Position = vec4(vertex_position, layer_z, 1.0);
}