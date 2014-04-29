#version 120

varying vec3 shared_color;

void main()
{	

	gl_FragColor = vec4(vec3(shared_color),1);
	

	
}
