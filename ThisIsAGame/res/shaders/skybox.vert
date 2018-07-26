#version 150

in vec3 in_posL;
in vec3 in_color;

uniform mat4 u_matrix;

out vec4 v_color;
out vec3 v_coord;

void main()
{
	vec4 posL = u_matrix * vec4(in_posL, 1.0);

	v_color = vec4(in_color.rgb, 1.0);
	v_coord = in_posL;

	gl_Position = posL;
}
   