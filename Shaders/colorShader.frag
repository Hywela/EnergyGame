uniform int particleNumLigth=0;
uniform int platformNumLitLigth=0;
uniform int platformNumUnlitLigth=0;

uniform vec2 lightpos[40];
uniform vec2 unlitPlatformLightpos[20];
uniform vec2 litPlatformLightpos[20];
uniform vec2 MainCharLightpos;

uniform vec3 lightColor;
uniform float screenHeight;
uniform vec3 lightAttenuation;
uniform float radius;

varying vec2 texCoord;
uniform sampler2D tex;

vec4 light (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=lightpos[i]-pixel;

	float distance=length(aux)/20;
		
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	

  return color;
}

vec4 mainCharLight ()
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=MainCharLightpos-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	

  return color*radius;
}

vec4 litPlatform (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=litPlatformLightpos[i]-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	

  return color*radius;
}

vec4 unlitPlatform (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=unlitPlatformLightpos[i]-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(vec3(gl_Color),1.0);	

  return color;
}

void main()
{	
vec4 sum = vec4(0,0,0,0);
vec4 lit = vec4(0,0,0,0);
vec4 unlit = vec4(0,0,0,0);

if(particleNumLigth >0){
for(int i = 0; i < particleNumLigth; i++){
sum+=light(i);

}
}
if(platformNumLitLigth >0){
for(int i = 0; i < platformNumLitLigth; i++){
	lit+=litPlatform(i);
}
}
if(platformNumUnlitLigth >0){
for(int i = 0; i < platformNumUnlitLigth; i++){
	unlit+=unlitPlatform(i);
}
}


	
	vec4 color = texture2D(tex, texCoord);
	gl_FragColor = sum+unlit+lit+mainCharLight()+color;
	

	
}
