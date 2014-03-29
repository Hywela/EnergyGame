
uniform vec2 lightpos[60];
uniform vec3 lightColor;
uniform float screenHeight;
uniform vec3 lightAttenuation;
uniform float radius;
uniform int numLigth;
//*uniform sampler2D texture;
vec4 light (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	
	vec2 aux=lightpos[i]-pixel;
	float distance=length(aux);
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	
	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	
	

 
 
  return color;
}
void main()
{	

vec4 calcColor[60];		
vec4 sum = vec4(0,0,0,0);
	for(int i = 0; i <20; i++){
	calcColor[i] =   light(i);
	
	}
	for(int i = 0; i <20; i++){
	sum +=  calcColor[i];

	}
	
	gl_FragColor = sum;
	

	
}
