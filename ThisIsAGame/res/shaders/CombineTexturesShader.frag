precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;


vec4 CombineTextures()
{
	float gamma = 2.2;
	float exposure = 1.0;
	vec3 scene = texture2D(u_texture_1, v_uv).rgb;
	vec3 bloom = texture2D(u_texture_0, v_uv).rgb;

	vec3 result = scene + bloom;

	// tone maping
	// vec3 result = vec3(1.0) - exp(-result * exposure);
	// result = pow(result, vec3(1.0 / gamma));

	return vec4(result, 1.0);
}

void main()
{
	gl_FragColor = CombineTextures();
}
