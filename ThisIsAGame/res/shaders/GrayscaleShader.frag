precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture_0;

vec4 Grayscale()
{
	vec4 tex = texture2D(u_texture_0, v_uv);

	float grayscale = dot(vec3(0.2126, 0.7152, 0.0722), tex.rgb);

	return vec4(grayscale, grayscale, grayscale, tex.a);
}

void main()
{
	gl_FragColor = Grayscale();
}