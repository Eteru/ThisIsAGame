
attribute vec3 a_posL;

varying vec2 v_uv;

void main()
{
	v_uv = (a_posL.rg + 1.0) * 0.5;
	gl_Position = vec4(a_posL, 1.0);
}
   