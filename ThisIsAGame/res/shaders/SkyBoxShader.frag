precision mediump float;
varying vec4 v_color;
varying vec3 v_coord;
uniform samplerCube u_cubeTexture;
void main()
{
	vec4 col = textureCube(u_cubeTexture, v_coord);
	if (col.a < 0.1) {
		discard;
	}

	gl_FragColor = col;
}
