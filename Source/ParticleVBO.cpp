#include "ParticleVBO.h"

void ParticleVBO::draw()
{

	if (vertices.size() > 0){
		setVBO();
		glEnable(GL_PROGRAM_POINT_SIZE_EXT);
		glPointSize(65);

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexPointer(2, GL_FLOAT, 0, 0);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, colorID);
		glColorPointer(3, GL_FLOAT, 0, 0);
		
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableClientState(GL_COLOR_ARRAY);
		//restore the GL state back
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode
		glDisable(GL_PROGRAM_POINT_SIZE_EXT);


	}

}

ParticleVBO::ParticleVBO()
{

	
}
ParticleVBO::~ParticleVBO()
{
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &colorID);

}

void ParticleVBO::setVBO()
{
	// Vertices:
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *colors.size(), &colors.front(), GL_STATIC_DRAW);

}
void ParticleVBO::setCenter(b2Vec2 center){
	this->center.x = center.x*M2P;
	this->center.y = center.y*M2P;
}
void ParticleVBO::pushBackCenter(b2Vec2 pcenter){
	postions.push_back(pcenter.x*M2P);
	postions.push_back(pcenter.y*M2P);
}
GLfloat *ParticleVBO::getCenter(){
	int s = postions.size();
	GLfloat *dd;
	dd = new GLfloat[s*2];
//	std::cout << " " <<s;
	for (int i = 0; i < s; i +=1){
		dd[i] = postions.at(i);
	}
	return dd;
}
b2Vec2 ParticleVBO::getMainCenter(){
	return center;
}
int ParticleVBO::getCenterSize(){
	return postions.size();
}
void ParticleVBO::pushBack(b2Vec2 pCenter, float angle, float radius, b2Vec3 color){
	//Push central point
	int moveX = pCenter.x*M2P;
	int moveY = pCenter.y*M2P;

	//postions.push_back(moveX);
	//postions.push_back(moveY);




	//Creat points for circle (fan around center)
	for (float i = 0.0; i <= 360; i += 360.0 / 1) {
		vertices.push_back(moveX);
		vertices.push_back(moveY);


		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
		float thisX = (cos(i * M_PI / 165) * (radius )) *M2P;
		float thisY = (sin(i * M_PI / 165) * (radius)) *M2P;
		vertices.push_back(thisX + (moveX));
		vertices.push_back(thisY + (moveY));

		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
		 thisX = (cos(i * M_PI / 180) * (radius)) *M2P;
		 thisY = (sin(i * M_PI / 180) * (radius)) *M2P;
		//Center
		vertices.push_back(thisX + (moveX));
		vertices.push_back(thisY + (moveY));
	
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	}
	
}
void ParticleVBO::clear(){
	vertices.clear();
	colors.clear();
	postions.clear();
}



