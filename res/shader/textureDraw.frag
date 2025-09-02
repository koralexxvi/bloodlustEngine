#version 430 core

layout(set = 2, binding = 0) uniform sampler2D tex;
layout(std140, set = 3, binding = 0) uniform ColourUniform {
    vec4 colour;
};

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColour;

void main() {
    fragColour = colour * texture(tex, texCoord);
}
