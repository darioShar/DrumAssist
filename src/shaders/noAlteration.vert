void main() {
	// the vertex position
    gl_Position = gl_Vertex;

    // the texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // the vertex color
    gl_FrontColor = gl_Color;
}