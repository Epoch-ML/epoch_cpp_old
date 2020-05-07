#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform UniformBufferObjectTransforms {
    mat4 m_mat4Model;
    mat4 m_mat4View;
    mat4 m_mat4Projection;
} u_uboTransforms;

void main() {
    gl_Position = 
        u_uboTransforms.m_mat4Projection * 
        u_uboTransforms.m_mat4View *  
        u_uboTransforms.m_mat4Model * 
        vec4(inPosition, 0.0, 1.0);

    fragColor = inColor;
}