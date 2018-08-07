#version 150

in vec2 v_uv;

uniform sampler2D u_texture_0;

// out value
out vec4 out_frag_color;

void main()
{
	out_frag_color = texture2D(u_texture_0, v_uv);
}
