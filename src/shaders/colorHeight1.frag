#define PI 3.1415926535897932384626433832795
uniform float height;

void main()
{
	if (height > 1) {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else {
		gl_FragColor = vec4( vec3( sin(height*PI/2.0), sin(height*PI), cos(height*PI/2.0)), 1.0 );
	}
}