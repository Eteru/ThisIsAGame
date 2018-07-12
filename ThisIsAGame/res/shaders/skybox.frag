#version 150
#extension GL_NV_shadow_samplers_cube : enable

in vec4 v_color;
in vec3 v_coord;

uniform samplerCube u_cubeTexture;

// out value
out vec4 out_frag_color;

void main()
{
	vec4 col = textureCube(u_cubeTexture, v_coord);
	if (col.a < 0.1) {
		discard;
	}

	out_frag_color = col;
}
