#version 330 core

// out value
out float fragment_depth;

void main()
{
	fragment_depth = gl_FragCoord.z;
}
