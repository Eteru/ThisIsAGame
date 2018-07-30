#version 150

in vec3 in_posL;
in vec3 in_normal;
in vec2 in_uv;
in vec2 in_uvBlend;

uniform mat4 u_matrix;
uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_light_space_matrix;

uniform sampler2D u_texture_3;

out float v_visibility;
out vec2 v_uv;
out vec2 v_uvBlend;
out vec3 v_normal;
out vec3 v_pos;
out vec4 v_pos_light_space;

const float DENSITY = 0.0035;
const float GRADIENT = 5.0;

void main()
{
	vec4 pos_ms = u_m * vec4(in_posL, 1.0);
	vec4 pos_ws = u_matrix * vec4(in_posL, 1.0);
	vec4 pos_cs = u_vm * vec4(in_posL, 1.0);
	
	v_pos = pos_ms.xyz;
	v_normal = (u_nm * vec4(in_normal, 1.0)).rgb;
	v_uv = in_uv;
	v_uvBlend = in_uvBlend;

	// fog
	float dist = abs(length(pos_cs.xyz));
	v_visibility = exp(-pow((dist * DENSITY), GRADIENT));
	v_visibility = clamp(v_visibility, 0.0, 1.0);
	
	//v_pos_light_space = u_light_space_matrix * u_m * posL;
	gl_Position = pos_ws;
}
   