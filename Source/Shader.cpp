#include "Shader.h"

Shader::Shader(const char* vsfn, const char* fsfn)
{
	FileReader readVert(vsfn);
	FileReader readFrag(fsfn);
	vertexShader	= glCreateShader(GL_VERTEX_SHADER);
	fragmentShader	= glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderSource = readVert.ReadFileAll();
	std::string fragmentShaderSource = readFrag.ReadFileAll();

	const char * vv = vertexShaderSource.c_str();
	const char * ff = fragmentShaderSource.c_str();

	glShaderSource(vertexShader, 1, &vv,NULL);
	glShaderSource(fragmentShader, 1, &ff,NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	printInfoLog(vertexShader);
	printInfoLog(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);

	glLinkProgram(shaderProgram);
	printInfoLog(shaderProgram);
};
Shader::Shader(const char* vsfn, const char* fsfn, const char* gsgn)
{
	FileReader readVert(vsfn);
	FileReader readFrag(fsfn);
	FileReader readGeo(gsgn);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	geomShader = glCreateShader(GL_GEOMETRY_SHADER);

	std::string vertexShaderSource = readVert.ReadFileAll();
	std::string fragmentShaderSource = readFrag.ReadFileAll();
	std::string geoShaderSource = readFrag.ReadFileAll();

	const char * vv = vertexShaderSource.c_str();
	const char * ff = fragmentShaderSource.c_str();
	const char * gg = geoShaderSource.c_str();

	glShaderSource(vertexShader, 1, &vv, NULL);
	glShaderSource(fragmentShader, 1, &ff, NULL);
	glShaderSource(geomShader, 1, &gg, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glCompileShader(geomShader);

	printInfoLog(vertexShader);
	printInfoLog(fragmentShader);
	printInfoLog(geomShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geomShader);

	glLinkProgram(shaderProgram);
	printInfoLog(shaderProgram);
};

std::string Shader::ReadFile(const char* filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	return 0;
};

void Shader::printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
};

GLhandleARB* Shader::GetShaderProgram()
{
	return &shaderProgram;
};