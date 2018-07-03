attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec2 a_uvBlend;

varying vec4 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;
varying vec3 v_normal;
varying vec3 v_pos;
varying vec4 v_pos_light_space;

uniform vec3 u_heights;
uniform mat4 u_matrix;
uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_light_space_matrix;


uniform sampler2D u_texture_3;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	
	v_normal = normalize(u_nm * vec4(a_normal, 1.0)).rgb;

	vec4 c_blend = texture2D(u_texture_3, a_uvBlend);
	//posL.y += c_blend.r * u_heights.r + c_blend.g * u_heights.g + c_blend.b * u_heights.b;
	posL = u_matrix * posL;

	v_color = vec4(a_color.rgb, 1.0);
	v_uv = a_uv;
	v_uvBlend = a_uvBlend;

	vec4 pos = u_m * vec4(a_posL, 1.0);
	
	v_pos = pos.xyz;
	v_pos_light_space = u_light_space_matrix * u_m * posL;
	gl_Position = posL;
}
   