#version 430 core

struct FragData {
    vec4 color;
    vec2 texCoord;
};

layout(set = 2, binding = 0) uniform sampler2D tex;

layout(location = 0) in FragData data;
layout(location = 0) out vec4 FragColor;

void main() {
    // float smoothing = 1.0 / 16.0;
    // float distance = texture(tex, data.texCoord).w;
    // float alpha = mix(0.5 - smoothing, 0.5 + smoothing, distance);
    // FragColor = vec4(data.color.xyz, data.color.w * alpha);

    FragColor = texture(tex, data.texCoord) * data.color;
}
