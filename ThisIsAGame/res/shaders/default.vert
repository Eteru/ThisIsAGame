#version 330

layout(location = 0) in vec3 in_posL;
in vec3 in_color;
in vec3 in_normal;
in vec3 in_binorm;
in vec3 in_tangent;
in vec2 in_uv;

uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_matrix;

out float v_visibility;
out vec2 v_uv;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_binorm;
out vec3 v_tangent;
out vec4 v_color;

const float DENSITY = 0.0035;
const float GRADIENT = 5.0;

void main()
{
	vec4 pos_ms = u_m * vec4(in_posL, 1.0);
	vec4 pos_ws = u_matrix * vec4(in_posL, 1.0);
	vec4 pos_cs = u_vm * vec4(in_posL, 1.0);
	
	//mat3 nm3 = mat3(u_nm);
	//v_normal = normalize(nm3 * in_normal);
	//v_binorm = normalize(nm3 * in_binorm);
	//v_tangent = normalize(nm3 * in_tangent);
	
	v_color = vec4(in_color.rgb, 1.0);
	v_normal = normalize(u_m * vec4(in_normal, 1.0)).rgb;
	v_uv = in_uv;
	v_pos = pos_ms.xyz;

		// fog
	float dist = abs(length(pos_cs.xyz));
	v_visibility = exp(-pow((dist * DENSITY), GRADIENT));
	v_visibility = clamp(v_visibility, 0.0, 1.0);

	gl_Position = pos_ws;
}
   