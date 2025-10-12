#version 430 core

layout(std140, set = 1, binding = 0) uniform UniformData {
    mat4 projection;
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec2 texCoord;

void main() {
    texCoord = vTexCoord;
    gl_Position = projection * position;
}
