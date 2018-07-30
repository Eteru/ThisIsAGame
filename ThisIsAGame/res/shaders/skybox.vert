#version 150

in vec3 in_posL;

uniform float u_time;
uniform mat4 u_matrix;

out float v_blend;
out vec3 v_coord;

const float TIME_1_DIV_24 = 0.04166666666f;

void main()
{
	vec4 posL = u_matrix * vec4(in_posL, 1.0);

	v_coord = in_posL;
	v_blend = u_time * TIME_1_DIV_24;

	gl_Position = posL;
}
   