
in vec3 in_posL;
in vec3 in_color;
in vec3 in_normal;
in vec2 in_uv;
in vec2 in_uvBlend;

varying vec4 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;
varying vec3 v_normal;
varying vec3 v_pos;
varying vec4 v_pos_light_space;

uniform mat4 u_matrix;
uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_light_space_matrix;


uniform sampler2D u_texture_3;

void main()
{
	vec4 posL = vec4(in_posL, 1.0);
	
	v_normal = normalize(u_nm * vec4(in_normal, 1.0)).rgb;

	vec4 c_blend = texture2D(u_texture_3, in_uvBlend);

	posL = u_matrix * posL;

	v_color = vec4(in_color.rgb, 1.0);
	v_uv = in_uv;
	v_uvBlend = in_uvBlend;

	vec4 pos = u_m * vec4(in_posL, 1.0);
	
	v_pos = pos.xyz;
	v_pos_light_space = u_light_space_matrix * u_m * posL;
	gl_Position = posL;
}
   