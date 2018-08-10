#version 330
#extension GL_NV_shadow_samplers_cube : enable

// declare structs
struct Fog
{
	float alpha;
	vec3 color;
};

struct AmbientLight
{
	vec3 color;
	float ratio;
};

struct Light
{
	vec3 type;
	float attenuation;

	vec3 pos;
	vec3 dir;
	
	// diffuse light
	vec3 diffusal;
	float diffusal_ratio;

	// specular light
	vec3 specular;
	float specular_ratio;
	float shininess;

	// spot_light
	float cone_angle;
};

in float v_depth;
in float v_visibility;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_pos;
in vec3 v_binorm;
in vec3 v_tangent;
in vec4 v_color;

uniform vec3 u_camera_pos;

// lights count
#define MAX_LIGHTS 20
uniform int u_lights_count;

// fog
uniform Fog u_fog;

// ambiental light
uniform AmbientLight u_ambiental;

uniform Light u_lights[MAX_LIGHTS];

// out value
out vec4 out_frag_color;

const float M_PI = 3.14159;

vec3 ComputeLight(Light light, vec3 l, vec3 surface, vec3 normal, vec3 eye, float attenuation)
{
	// ambient
	vec3 IA = u_ambiental.ratio * u_ambiental.color * surface;

	// diffuse
	float diff_coef = light.diffusal_ratio * max(0.0, dot(normal, l));
	vec3 ID = diff_coef * light.diffusal * surface;

	// specular
	vec3 R = reflect(l, normal);
	float int_spec = max(dot(R, eye), 0.0);
	float spec_coef = light.specular_ratio * pow(int_spec, light.shininess);

	vec3 IS = spec_coef * light.specular * surface;

	return IA + attenuation * (ID + IS);
}

vec3 ApplyLight(Light light, vec3 surface, vec3 normal, vec3 eye)
{
	vec3 l = vec3(0.0);
	float attenuation = 1.0;

	// directional
	l = normalize(light.dir - light.pos);
	vec3 dir_light = ComputeLight(light, l, surface, normal, eye, attenuation);

	// point
	l = normalize(light.pos - v_pos);

	float dist_to_light = length(light.pos - v_pos);
	attenuation = 1.0 / (1.0 + light.attenuation * pow(dist_to_light, 2.0));

	vec3 point_light = ComputeLight(light, l, surface, normal, eye, attenuation);

	// spot
	float AoI = acos(dot(-l, normalize(light.dir)));
	attenuation = cos(clamp(AoI * M_PI * 0.5 / light.cone_angle, 0.0, M_PI * 0.5));

	vec3 spot_light = ComputeLight(light, l, surface, normal, eye, attenuation);

	return light.type.r * point_light + light.type.g * dir_light + light.type.b * spot_light;
}

vec3 ApplyFog(vec3 color)
{
	return mix(u_fog.color, color, v_visibility);
}

vec3 ApplyMurkiness(vec3 color)
{
	const float max_depth = 10.f;
	const vec3 deep_water = vec3(0.031, 0.101, 0.349);

	float depth_factor = 1.0 - smoothstep(0, max_depth, v_depth);

	return mix(deep_water, color, depth_factor);
}

void main()
{
	const vec4 water_color = vec4(0.678, 0.847, 0.901, 0.6);

	//vec3 e = normalize(u_camera_pos - v_pos);
	//vec3 n = normalize(v_normal);
	
	//tex = ApplySkyboxReflection(tex, n, e);
	
	//vec3 light_col = vec3(0.0);
	//light_col += ApplyLight(u_lights[0], water_color.rgb, n, e);
	//light_col += ApplyLight(u_lights[1], tex.rgb, n, e);
	
	//final color (after gamma correction)
    //vec3 gamma = vec3(1.0 / 2.2);
	//light_col = pow(light_col, gamma);
	
	vec3 murky_water = ApplyMurkiness(water_color.rgb);
	out_frag_color = vec4(ApplyFog(murky_water), water_color.a);
}
