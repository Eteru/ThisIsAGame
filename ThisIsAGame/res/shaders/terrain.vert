#version 150

in vec3 in_posL;
in vec3 in_color;
in vec3 in_normal;
in vec2 in_uv;
in vec2 in_uvBlend;

uniform mat4 u_matrix;
uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_light_space_matrix;

uniform sampler2D u_texture_3;

out vec4 v_color;
out vec2 v_uv;
out vec2 v_uvBlend;
out vec3 v_normal;
out vec3 v_pos;
out vec4 v_pos_light_space;

void main()
{
	vec4 pos_ms = u_m * vec4(in_posL, 1.0);
	vec4 pos_ws = u_matrix * vec4(in_posL, 1.0);
	
	v_normal = normalize(u_nm * vec4(in_normal, 1.0)).rgb;

	vec4 c_blend = texture2D(u_texture_3, in_uvBlend);
	
	v_color = vec4(in_color.rgb, 1.0);
	v_uv = in_uv;
	v_uvBlend = in_uvBlend;

	
	v_pos = pos_ms.xyz;
	//v_pos_light_space = u_light_space_matrix * u_m * posL;
	gl_Position = pos_ws;
}
   