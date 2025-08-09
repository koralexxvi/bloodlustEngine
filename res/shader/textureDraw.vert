#version 430 core

layout(std140, set = 1, binding = 0) uniform UniformData {
    mat4 projection;
    mat4 model;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec2 texCoord;

void main() {
    texCoord = vTexCoord;
    gl_Position = (projection * model) * vec4(position, 0.0f, 1.0f);
}
