                                                          
     varying vec4 lightColor;   
	 varying vec2 texCoord;                                                                            
void main()                                                                         
{       
	texCoord = gl_MultiTexCoord0.xy;                                                                            
    gl_FrontColor = gl_Color;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                           
                             
}