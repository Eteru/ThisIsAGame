
attribute vec3 a_posL;

varying vec4 v_color;

uniform vec3 u_color;
uniform mat4 u_matrix;

void main()
{
	vec4 posL = u_matrix * vec4(a_posL, 1.0);

	v_color = vec4(u_color, 1.0);

	gl_Position = posL;
}
   