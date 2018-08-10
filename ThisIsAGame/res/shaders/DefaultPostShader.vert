#version 330

layout(location = 0) in vec3 in_posL;

out vec2 v_uv;

void main()
{
	v_uv = (in_posL.rg + 1.0) * 0.5;
	gl_Position = vec4(in_posL, 1.0);
}
   