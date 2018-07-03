precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture_0;

vec4 Threshold()
{
	vec4 tex = texture2D(u_texture_0, v_uv);
	float Y = 0.2126 * tex.r + 0.7152 * tex.g + 0.0722 * tex.b;

	if (Y < 0.7) {
		return vec4(0.0, 0.0, 0.0, 1.0);
	}

	return tex;
}

void main()
{
	gl_FragColor = Threshold();
}
