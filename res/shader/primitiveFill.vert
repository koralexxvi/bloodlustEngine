#version 430 core

struct InstanceData {
    mat4 transform;
    vec4 color;
};

layout(set = 0, binding = 0) readonly restrict buffer instanceData {
    InstanceData data[8192];
};

layout(std140, set = 1, binding = 0) uniform matrices {
    mat4 projection;
};

layout(location = 0) in vec4 vPos;

layout(location = 0) out vec4 color;

void main() {
    InstanceData instance = data[gl_InstanceIndex];
    gl_Position = (projection * instance.transform) * vPos;
    color = instance.color;
}
