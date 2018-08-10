#version 330

in vec2 in_uv;
layout(location = 0) in vec3 in_posL;
in vec3 in_color;
in vec3 in_normal;
in vec3 in_binorm;
in vec3 in_tangent;
in vec4 in_indicators;

uniform float u_time;
uniform mat4 u_m;
uniform mat4 u_nm;
uniform mat4 u_vm;
uniform mat4 u_matrix;

out float v_depth;
out float v_visibility;
out vec2 v_uv;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_binorm;
out vec3 v_tangent;
out vec4 v_color;

const float M_PI = 3.14159;

const float DENSITY = 0.0035;
const float GRADIENT = 5.0;
const float WAVE_LENGTH = 4.0;
const float WAVE_AMPLITUDE = 0.2;

float GenerateOffset(float x, float z, float val1, float val2)
{
	float r_x = ((mod(x+z*x*val1, WAVE_LENGTH)/WAVE_LENGTH) + u_time * mod(x * 0.8 + z, 1.5)) * 2.0 * M_PI;
	float r_z = ((mod(val2 * (z*x +x*z), WAVE_LENGTH)/WAVE_LENGTH) + u_time * 2.0 * mod(x , 2.0) ) * 2.0 * M_PI;

	return WAVE_AMPLITUDE * 0.5 * (sin(r_z) + cos(r_x));
}

vec3 ApplyDistortion(vec3 vertex)
{
	float distortion_x = GenerateOffset(vertex.x, vertex.z, 0.2, 0.1);
	float distortion_y = GenerateOffset(vertex.x, vertex.z, 0.1, 0.3);
	float distortion_z = GenerateOffset(vertex.x, vertex.z, 0.15, 0.2);

	return vertex + vec3(distortion_x, distortion_y, distortion_z);
}

vec3 CalcNormal(vec3 v0, vec3 v1, vec3 v2)
{
	vec3 tg = v1 - v0;
	vec3 bitg = v2 - v0;

	return normalize(cross(tg, bitg));
}

void main()
{
	vec3 pos = vec3(in_posL.x, 0.0, in_posL.z);

	// the other 2 vertices
	vec3 v1 = pos + vec3(in_indicators.x, 0.0, in_indicators.y);
	vec3 v2 = pos + vec3(in_indicators.z, 0.0, in_indicators.w);

	// distortion
	pos = ApplyDistortion(pos);
	v1 = ApplyDistortion(v1);
	v2 = ApplyDistortion(v2);

	v_normal = CalcNormal(pos, v1, v2);

	vec4 pos_ms = u_m * vec4(pos, 1.0);
	vec4 pos_ws = u_matrix * vec4(pos, 1.0);
	vec4 pos_cs = u_vm * vec4(pos, 1.0);
	
	//mat3 nm3 = mat3(u_nm);
	//v_normal = normalize(nm3 * in_normal);
	//v_binorm = normalize(nm3 * in_binorm);
	//v_tangent = normalize(nm3 * in_tangent);
	
	v_color = vec4(in_color.rgb, 1.0);
	v_uv = in_uv;
	v_pos = pos_ms.xyz;

	// fog
	float dist = abs(length(pos_cs.xyz));
	v_visibility = exp(-pow((dist * DENSITY), GRADIENT));
	v_visibility = clamp(v_visibility, 0.0, 1.0);

	v_depth = -min(0.0, in_posL.y);

	gl_Position = pos_ws;
}
   