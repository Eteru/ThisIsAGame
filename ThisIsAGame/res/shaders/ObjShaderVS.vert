precision mediump float;
attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec3 a_binorm;
attribute vec3 a_tangent;
attribute vec2 a_uv;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_pos;
varying vec3 v_binorm;
varying vec3 v_tangent;
varying vec4 v_color;

uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_matrix;

void main()
{
	vec4 posL = u_matrix * vec4(a_posL, 1.0);
	
	mat3 nm3 = mat3(u_nm);
	v_normal = normalize(nm3 * a_normal);
	v_binorm = normalize(nm3 * a_binorm);
	v_tangent = normalize(nm3 * a_tangent);

	vec4 pos = u_m * vec4(a_posL, 1.0);

	v_color = vec4(a_color.rgb, 1.0);
	v_uv = a_uv;

	v_pos = pos.xyz;
	gl_Position = posL;
}
   