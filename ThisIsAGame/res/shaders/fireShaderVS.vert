attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv;
varying vec4 v_color;
varying vec2 v_uv;
uniform mat4 u_matrix;

void main()
{
v_color = vec4(a_color.rgb, 1.0);
vec4 posL = u_matrix*vec4(a_posL, 1.0);
v_uv = a_uv;
gl_Position = posL;
}
   