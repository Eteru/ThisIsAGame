precision mediump float;
varying vec4 v_color;
varying vec2 v_uv;

uniform float u_time;
uniform float u_disp_max;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

vec2 remap_interval(vec2 v)
{
	vec2 ret;
	ret.x = -u_disp_max + v.x * (u_disp_max + u_disp_max);
	ret.y = -u_disp_max + v.y * (u_disp_max + u_disp_max);

	return ret;
}

void main()
{
	vec2 disp = texture2D(u_texture_2, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = remap_interval(disp);
	vec2 v_uv_displaced = v_uv + offset;
	vec4 c_fire = texture2D(u_texture_1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture_0, v_uv);
	c_fire.a *= c_alpha.r;

	if (c_fire.a < 0.1) {
		discard;
	}

	gl_FragColor = c_fire;
}
