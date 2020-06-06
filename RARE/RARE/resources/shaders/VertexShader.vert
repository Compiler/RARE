#version 460
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec3 fragColor;

float a  = 0.5;
vec2 positions[3] = vec2[](
	vec2(-a , a),			//bottom left
	vec2( a, -a)	,		//top left
	vec2(-a, -a)	);			//bottom right
	//vec2(0,0),				//bottom left
	//vec2(0,0)	,			//bottom left
	//vec2(0,0)	);			//bottom left

vec3 colors[6] = vec3[](
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0),
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0));
void main() {

	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	fragColor = colors[gl_VertexIndex];

}