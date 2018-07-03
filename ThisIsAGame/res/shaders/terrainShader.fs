precision mediump float;

varying vec3 v_normal;
varying vec3 v_pos;
varying vec4 v_pos_light_space;
varying vec4 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;

uniform vec3 u_camera_pos;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_shadow_map;

// fog
uniform struct Fog
{
	float alpha;
	vec3 color;
} u_fog;

// lights count
#define MAX_LIGHTS 20
uniform highp int u_lights_count;

// ambiental light
uniform struct AmbientLight
{
	vec3 color;
	float ratio;
} u_ambiental;

uniform struct Light
{
	highp vec3 type;
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
} u_lights[MAX_LIGHTS];

#define M_PI 3.1415926535897932384626433832795

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
	vec3 IA = u_ambiental.ratio * surface * u_ambiental.color;

	// diffuse
	float diff_coef = light.diffusal_ratio * max(0.0, dot(normal, l));
	vec3 ID = diff_coef * surface * light.diffusal;

	// specular
	vec3 R = reflect(l, normal);
	float int_spec = max(dot(R, eye), 0.0);
	float spec_coef = light.specular_ratio * pow(int_spec, light.shininess);

	float shadow = 0.0;//ShadowPercentage();
	vec3 IS = spec_coef * surface * light.specular;

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
	float alpha_clamped = clamp(u_fog.alpha, 0.0, 1.0);
	
	return alpha_clamped * u_fog.color + (1.0 - alpha_clamped) * color.xyz;
}

void main()
{
	vec4 c_1 = texture2D(u_texture_0, v_uv);
	vec4 c_2 = texture2D(u_texture_1, v_uv);
	vec4 c_3 = texture2D(u_texture_2, v_uv);
	vec4 c_A = texture2D(u_texture_3, v_uvBlend);

	vec4 c_final = c_A.r * c_1 + c_A.g * c_2 + c_A.b * c_3;
	c_final.a = 1.0;

	vec3 e = normalize(u_camera_pos - v_pos);
	vec3 n = normalize(v_normal);
	
	vec3 light_col = vec3(0.0);
	light_col += ApplyLight(u_lights[0], c_final.rgb, n, e);
	light_col += ApplyLight(u_lights[1], c_final.rgb, n, e);
	light_col += ApplyLight(u_lights[2], c_final.rgb, n, e);

	//final color (after gamma correction)
    vec3 gamma = vec3(1.0 / 2.2);
	light_col = pow(light_col, gamma);

	gl_FragColor = vec4(ApplyFog(light_col), c_final.a);
}
