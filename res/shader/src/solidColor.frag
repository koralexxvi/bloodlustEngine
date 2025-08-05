#version 430 core

layout(location = 0) in vec4 color;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(1.0f, 1.0f, 1.0f, gl_FragDepth);
}
