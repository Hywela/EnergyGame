
uniform vec2 lightpos[50];
uniform vec2 platformLightpos[12];
uniform vec2 MainCharLightpos;

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

	float distance=length(aux)/4;

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	

  return color*60;
}
vec4 mainCharLight ()
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=MainCharLightpos-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(lightColor,1.0);	

  return color*radius;
}
vec4 platFormLight (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=platformLightpos[i]-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	
	

 
 
  return color*60;
}
void main()
{	

vec4 calcColor[60];	
vec4 platcalcColor[10];
vec4 sum = vec4(0,0,0,0);
vec4 psum = vec4(0,0,0,0);



for(int i = 0; i <6; i++){
if (platformLightpos[i] != vec2(0,0))
	platcalcColor[i] =   platFormLight(i);
	
	}
	for(int i = 0; i <6; i++){
	if (platformLightpos[i] != vec2(0,0))
	psum +=  platcalcColor[i];

	}
	


	for(int i = 0; i <40; i++){
	if (lightpos[i] != vec2(0,0))
	calcColor[i] =   light(i);
	
	}
	for(int i = 0; i <40; i++){
		if (lightpos[i] != vec2(0,0))
	sum +=  calcColor[i];

	}
	

	gl_FragColor = sum+psum+mainCharLight();
	

	
}
