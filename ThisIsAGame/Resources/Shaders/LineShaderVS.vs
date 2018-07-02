attribute vec3 a_posL;
attribute vec3 a_color;
varying vec4 v_color;
uniform mat4 u_matrix;
void main()
{
v_color = vec4(a_color.rgb, 1.0);
vec4 posL = vec4(a_posL, 1.0);
gl_Position = posL;
}
   