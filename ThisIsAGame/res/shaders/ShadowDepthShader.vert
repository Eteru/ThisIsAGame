
attribute vec3 a_posL;

uniform mat4 u_m;
uniform mat4 u_light_space_matrix;

void main()
{
	vec4 pos = u_light_space_matrix * u_m * vec4(a_posL, 1.0);
	pos.z = (pos.z + 1.0) * 0.5;
	gl_Position = pos;
}
   