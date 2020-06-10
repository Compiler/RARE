#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    float time;
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
layout(binding = 1) uniform sampler2D u_texSampler;

layout(location = 0) in vec3 o_fragColor;
layout(location = 1) in vec2 o_texCoord;

layout(location = 0) out vec4 outColor;

void main(){
	outColor = vec4(o_texCoord, 0.0, 1.0);
    outColor = texture(u_texSampler, o_texCoord) * outColor + 0.1;
   
}
