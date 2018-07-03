attribute vec3 a_posL;
attribute vec3 a_color;
varying vec4 v_color;
varying vec3 v_coord;
uniform mat4 u_matrix;

void main()
{
	v_color = vec4(a_color.rgb, 1.0);
	vec4 posL = u_matrix*vec4(a_posL, 1.0);
	v_coord = a_posL;
	gl_Position = posL;
}
   