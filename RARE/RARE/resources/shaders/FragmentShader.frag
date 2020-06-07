#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float time;
} ubo;

layout(location = 0) in vec3 o_fragColor;


layout(location = 0) out vec4 outColor;

void main(){
	outColor = vec4(o_fragColor, 1.0);
   
}
