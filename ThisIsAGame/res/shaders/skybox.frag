#version 330
#extension GL_NV_shadow_samplers_cube : enable

struct Fog
{
	float alpha;
	vec3 color;
};

in float v_blend;
in vec3 v_coord;

// fog
uniform Fog u_fog;

uniform samplerCube u_cube_texture_0;
uniform samplerCube u_cube_texture_1;

// out value
out vec4 out_frag_color;

const float LOWER_LIMIT = 0.0;
const float UPPER_LIMIT = 0.1;

void main()
{
	vec4 col_0 = textureCube(u_cube_texture_0, v_coord);
	vec4 col_1 = textureCube(u_cube_texture_1, v_coord);
	vec4 color = mix(col_0, col_1, v_blend);

	float factor = (v_coord.y - LOWER_LIMIT) / (UPPER_LIMIT - LOWER_LIMIT);
	factor = clamp(factor, 0.0, 1.0);

	out_frag_color =  mix(vec4(u_fog.color, 1.0), color, factor);
}
