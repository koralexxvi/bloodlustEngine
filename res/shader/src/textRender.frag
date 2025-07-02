#version 430 core

struct FragData {
    vec4 color;
    vec2 texCoord;
};

layout(set = 2, binding = 0) uniform sampler2D tex;

layout(location = 0) in FragData data;
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = data.color * texture(tex, data.texCoord);
}
