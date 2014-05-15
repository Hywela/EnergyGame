#version 150 compatibility
uniform int particleNumLigth=0;
uniform int platformNumLitLigth=0;
uniform int platformNumUnlitLigth=0;

uniform vec2 lightpos[40];
uniform vec2 unlitPlatformLightpos[20];
uniform vec2 litPlatformLightpos[20];
uniform vec2 MainCharLightpos;

uniform vec3 unlitLightColor;
uniform vec3 litLightColor;
uniform float screenHeight;
uniform vec3 lightAttenuation;
uniform float radius;


uniform sampler2D tex;

vec4 light (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=lightpos[i]-pixel;

	float distance=length(aux)/10;
		
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color1=vec4(attenuation,attenuation,attenuation,1.0);	
	
	vec4 color  = texture2D(tex, gl_TexCoord[1].st)*color1;
  return color;
}

vec4 mainCharLight ()
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=MainCharLightpos-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color  = texture2D(sampler2D, gl_TexCoord[1].xy);
  return color;
}

vec4 litPlatform (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=litPlatformLightpos[i]-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

		vec4 color1=vec4(attenuation,attenuation,attenuation,1.0);	
	
	vec4 color  = texture2D(tex, gl_TexCoord[1].st)*color1*vec4(litLightColor,1)*radius;
  return color;
}

vec4 unlitPlatform (int i)
{
vec2 pixel=gl_FragCoord.xy;		
	pixel.y=screenHeight-pixel.y;	

	vec2 aux=unlitPlatformLightpos[i]-pixel;

	float distance=length(aux);

	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	

	vec4 color1=vec4(attenuation,attenuation,attenuation,1.0);	
	
vec4 color  = texture2D(tex, gl_TexCoord[1].st)*color1*vec4(unlitLightColor,1);

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

	
	
	gl_FragColor = mainCharLight();
	

	
}
