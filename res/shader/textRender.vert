#version 430 core

struct FragData {
    vec4 colour;
    vec2 texCoord;
};

layout(std140, set = 1, binding = 0) uniform uniforms {
    mat4 projection;
    mat4 model;
} uniformData;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out FragData fragData;

void main() {
    fragData.colour = colour;
    fragData.texCoord = texCoord;
    gl_Position = (uniformData.projection * uniformData.model) * position;
}
