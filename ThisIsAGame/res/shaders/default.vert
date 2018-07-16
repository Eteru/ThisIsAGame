#version 150

in vec3 in_posL;
in vec3 in_color;
in vec3 in_normal;
in vec3 in_binorm;
in vec3 in_tangent;
in vec2 in_uv;

uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_matrix;

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_binorm;
out vec3 v_tangent;
out vec4 v_color;


void main()
{
	vec4 pos_ms = u_m * vec4(in_posL, 1.0);
	vec4 pos_ws = u_matrix * vec4(in_posL, 1.0);
	
	//mat3 nm3 = mat3(u_nm);
	//v_normal = normalize(nm3 * in_normal);
	//v_binorm = normalize(nm3 * in_binorm);
	//v_tangent = normalize(nm3 * in_tangent);
	
	v_color = vec4(in_color.rgb, 1.0);
	v_normal = in_normal;
	v_uv = in_uv;

	v_pos = pos_ms.xyz;
	gl_Position = pos_ws;
}
   