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

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;

uniform samplerCube u_cubeTexture;

// fog
uniform Fog u_fog;

// ambiental light
uniform AmbientLight u_ambiental;

uniform Light u_lights[MAX_LIGHTS];

// out value
out vec4 out_frag_color;

#define M_PI 3.1415926535897932384626433832795

vec3 ComputeLight(Light light, vec3 l, vec3 surface, vec3 normal, vec3 eye, float attenuation)
{
	// ambient
	vec3 IA = u_ambiental.ratio * surface * u_ambiental.color;

	// diffuse
	float diff_coef = light.diffusal_ratio * max(0.0, dot(normal, l));
	vec3 ID = diff_coef * surface * light.diffusal;

	// specular
	vec3 R = reflect(l, normal);
	float int_spec = max(dot(R, eye), 0.0);
	float spec_coef = light.specular_ratio * pow(int_spec, light.shininess);

	vec3 IS = spec_coef * surface * light.specular;

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
	float alpha_clamped = clamp(u_fog.alpha, 0.0, 1.0);
	
	return alpha_clamped * u_fog.color + (1.0 - alpha_clamped) * color.xyz;
}

vec3 ApplyNormalMap()
{
	vec3 normal_tex = normalize(texture2D(u_texture_1, v_uv).rgb * 2.0 - 1.0);
	mat3 TBN = mat3(v_tangent, v_binorm, v_normal);

	return normalize(TBN * normal_tex);
}

vec4 ApplySkyboxReflection(vec4 surface, vec3 n, vec3 e)
{
	vec3 dirReflect = reflect(e, n);
	vec4 sb = textureCube(u_cubeTexture, dirReflect);

	return surface * 0.7 + sb * 0.3;
}

void main()
{
	vec4 tex = texture2D(u_texture_0, v_uv);
	if (tex.a < 0.1) 
	{
		discard;
	}

	vec3 e = normalize(u_camera_pos - v_pos);
	vec3 n = normalize(v_normal + ApplyNormalMap());
	
	//tex = ApplySkyboxReflection(tex, n, e);
	
	vec3 light_col = vec3(0.0);
	light_col += ApplyLight(u_lights[0], tex.rgb, n, e);
	//light_col += ApplyLight(u_lights[1], tex.rgb, n, e);
	
	//final color (after gamma correction)
    vec3 gamma = vec3(1.0 / 2.2);
	light_col = pow(light_col, gamma);
	
	out_frag_color = vec4(light_col, 1.0);
	//out_frag_color = vec4(ApplyFog(light_col), 1.0);
}
