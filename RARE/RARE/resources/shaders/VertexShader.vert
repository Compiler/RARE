#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 o_fragColor;

void main() {

	gl_Position = vec4(a_position, 1.0);
	o_fragColor = a_color;

}