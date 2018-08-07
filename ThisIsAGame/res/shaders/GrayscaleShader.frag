#version 150

in vec2 v_uv;

uniform sampler2D u_texture_0;

// out value
out vec4 out_frag_color;

vec4 Grayscale()
{
	vec4 tex = texture2D(u_texture_0, v_uv);

	float grayscale = dot(vec3(0.2126, 0.7152, 0.0722), tex.rgb);

	return vec4(grayscale, grayscale, grayscale, tex.a);
}

void main()
{
	out_frag_color = Grayscale();
}
