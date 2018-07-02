precision mediump float;

varying vec2 v_uv;

uniform struct FragmentOffset
{
	float x;
	float y;
} u_fragment_offset;

uniform sampler2D u_texture_0;

#define KERNEL_SIZE 9

// Laplace
vec4 Sharpen()
{
	vec2 dxy[KERNEL_SIZE];
	dxy[0] = vec2(-1.0, -1.0);
	dxy[1] = vec2(0.0, -1.0);
	dxy[2] = vec2(1.0, -1.0);
	dxy[3] = vec2(-1.0, 0.0);
	dxy[4] = vec2(0.0, 0.0);
	dxy[5] = vec2(1.0, 0.0);
	dxy[6] = vec2(-1.0, 1.0);
	dxy[7] = vec2(0.0, 1.0);
	dxy[8] = vec2(1.0, 1.0);

	vec2 offset = vec2(u_fragment_offset.x, u_fragment_offset.y);

	vec4 accumulator = vec4(0.0);

	accumulator -= texture2D(u_texture_0, v_uv + dxy[0] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[1] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[2] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[3] * offset) * 0.111111;
	accumulator += texture2D(u_texture_0, v_uv + dxy[4] * offset) * 0.888888;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[5] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[6] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[7] * offset) * 0.111111;
	accumulator -= texture2D(u_texture_0, v_uv + dxy[8] * offset) * 0.111111;

	return accumulator;
}

void main()
{
	gl_FragColor = Sharpen();
}
