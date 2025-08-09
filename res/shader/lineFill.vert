#version 430 core

layout(std140, set = 1, binding = 0) uniform matrices {
    mat4 projection;
};

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 fColor;

void main() {
    gl_Position = projection * vec4(pos, 1.0f);
    fColor = color;
}
