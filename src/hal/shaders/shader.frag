#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    //outColor = fragColor;
    //outColor = vec4(fragTexCoord, 0.0, 1.0);
    outColor = max(vec4(0.025f), texture(texSampler, fragTexCoord));
}