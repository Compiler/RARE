#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    float time;
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texCoord;


layout(location = 0) out vec3 o_fragColor;
layout(location = 1) out vec2 o_texCoord;


void main() {

	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(a_position, 1.0);
	o_fragColor = a_color;
    o_texCoord = a_texCoord;

}