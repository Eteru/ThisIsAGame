#version 150
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

in float v_visibility;
in vec3 v_normal;
in vec3 v_pos;
in vec4 v_pos_light_space;
in vec2 v_uv;
in vec2 v_uvBlend;

uniform vec3 u_camera_pos;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_shadow_map;

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

float ShadowPercentage()
{
	vec3 proj_coords = v_pos_light_space.xyz / v_pos_light_space.w;
	proj_coords = proj_coords * 0.5 + 0.5;

	float closest_depth = texture2D(u_texture_shadow_map, proj_coords.xy).r; 

    return step(closest_depth, proj_coords.z);
}

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

	float shadow = 0.0;//ShadowPercentage();
	vec3 IS = spec_coef * light.specular * surface;

	return IA + (1.0 - shadow) * attenuation * (ID + IS);
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

void main()
{
	//const vec3 start_color = vec3(0.286, 0.219, 0.16);
	//const vec3 end_color = vec3(1.0, 1.0, 1.0);
	const vec3 start_color = vec3(0.0);
	const vec3 end_color = vec3(1.0);

	float height_perc = clamp(v_pos.y + 10.f, 0.0, 20.0) / 20.0;
	vec3 color = mix(start_color, end_color, height_perc);

	vec4 c_final = vec4(color, 1.0);
	
	vec3 e = normalize(u_camera_pos - v_pos);
	vec3 n = normalize(v_normal);
	
	vec3 light_col = vec3(0.0);
	light_col += ApplyLight(u_lights[0], c_final.rgb, n, e);
	//light_col += ApplyLight(u_lights[1], c_final.rgb, n, e);
	//light_col += ApplyLight(u_lights[2], c_final.rgb, n, e);
	
	////final color (after gamma correction)
    vec3 gamma = vec3(1.0 / 2.2);
	light_col = pow(light_col, gamma);
	
	out_frag_color = vec4(ApplyFog(light_col), c_final.a);
}
