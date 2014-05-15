#version 150

uniform sampler2D plattex;
uniform int textureFlag;

varying vec3 shared_color;
varying vec2 shared_texCoord;

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
out vec4 outPut;

vec4 getTexture(){
vec4 texture = texture2D(plattex, shared_texCoord);
return texture;
}

vec4 calcLigthMap (in int distanceShine,  in float sizeOfLigth , in int lp){
	vec2 pixel=gl_FragCoord.xy;	
	pixel.y=screenHeight-pixel.y;
	vec2 aux=lightpos[lp]-pixel;
	float distance=length(aux)/distanceShine;
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	
	vec4 outputMap = (vec4(attenuation,attenuation,attenuation,1.0) *getTexture() ) * sizeOfLigth ;
return outputMap;
}
vec4 mainCharLight ()
{
	vec2 pixel=gl_FragCoord.xy;	
	pixel.y=screenHeight-pixel.y;	
	vec2 aux=MainCharLightpos-pixel;
	float distance=length(aux);
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	
	vec4 ligth=vec4(attenuation,attenuation,attenuation,1.0);	
	vec4 outPutLigth  = getTexture()*ligth;
  return outPutLigth;
}
vec4 litPlatform (in int distanceShine,  in float sizeOfLigth , in int lp){
	vec2 pixel=gl_FragCoord.xy;	
	pixel.y=screenHeight-pixel.y;
	vec2 aux=litPlatformLightpos[lp]-pixel;
	float distance=length(aux)/distanceShine;
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	
	vec4 outputMap = (vec4(attenuation,attenuation,attenuation,1.0) *getTexture() ) * sizeOfLigth ;
return outputMap;
}

vec4 unlitPlatform (in int distanceShine,  in float sizeOfLigth , in int lp){
	vec2 pixel=gl_FragCoord.xy;	
	pixel.y=screenHeight-pixel.y;
	vec2 aux=unlitPlatformLightpos[lp]-pixel;
	float distance=length(aux)/distanceShine;
	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);	
	vec4 outputMap = (vec4(attenuation,attenuation,attenuation,1.0) *getTexture() ) * sizeOfLigth ;
return outputMap;
}

void main()
{		
vec4 sum = vec4(0,0,0,0);
vec4 lit = vec4(0,0,0,0);
vec4 unlit = vec4(0,0,0,0);
float radiusSize = 2 ;
int standard = 10;
for(int i = 0; i < particleNumLigth; i++){
	sum+=calcLigthMap(standard, radiusSize,  i);
}
 standard = 1;
radiusSize = radius;
for(int i = 0; i < platformNumLitLigth; i++){

	lit+=litPlatform(standard, radiusSize,  i)*vec4(litLightColor,1);
}
radiusSize = radius/2;
for(int i = 0; i < platformNumUnlitLigth; i++){

	unlit+=unlitPlatform(standard, radiusSize, i)*vec4(unlitLightColor,1);;
}

	outPut = sum+unlit+lit+mainCharLight();
	//outPut = getTexture();
}
