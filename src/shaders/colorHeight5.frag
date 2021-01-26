#define PI 3.1415926535897932384626433832795
uniform float height;

void main()
{
	float param1 = sin(height*PI/2);
	float param2 = 1.0 -  sin(height*PI/2);
	float param3 = sin(height*PI/2 + PI/4);

	gl_FragColor = vec4(param3, param2, param1, 1.0);

}