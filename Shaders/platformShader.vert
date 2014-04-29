
attribute vec3 vertex;

attribute vec2 texCoord;



varying vec2 shared_texCoord;
varying vec3 shared_normal;
                                                                          
void main()                                                                         
{        
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex,1.0);
	//Send data to fragment shader
	
	shared_texCoord = texCoord;

                             
}
