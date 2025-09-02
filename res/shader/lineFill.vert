#version 430 core

layout(std140, set = 1, binding = 0) uniform matrices {
    mat4 projection;
};

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 colour;

layout(location = 0) out vec4 fColour;

void main() {
    gl_Position = projection * pos;
    fColour = colour;
}
