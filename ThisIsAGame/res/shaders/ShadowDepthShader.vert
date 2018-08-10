#version 330

layout(location = 0) in vec3 in_posL;

uniform mat4 u_m;
uniform mat4 u_light_space_matrix;

void main()
{
	vec4 pos = u_light_space_matrix * u_m * vec4(in_posL, 1.0);

	gl_Position = pos;
}
   