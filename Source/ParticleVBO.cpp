#include "ParticleVBO.h"

void ParticleVBO::draw()
{

	if (vertices.size() > 0){
		setVBO();
		
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexPointer(2, GL_FLOAT, 0, 0);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, colorID);
		glColorPointer(3, GL_FLOAT, 0, 0);
		
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()/2);

	
		//restore the GL state back
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode
		

	}

}

ParticleVBO::ParticleVBO()
{
	// Vertices:
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &colorID);
}
ParticleVBO::~ParticleVBO()
{
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &colorID);

}

void ParticleVBO::setVBO()
{
	// Vertices:
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);

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
	GLfloat *temp;
	temp = new GLfloat[s];
//	std::cout << " " <<s;
	for (int i = 0; i < s; i +=1){
		temp[i] = postions.at(i);
	}
	return temp;
}
b2Vec2 ParticleVBO::getMainCenter(){
	return center;
}
int ParticleVBO::getCenterSize(){
	return postions.size()/2;
}
void ParticleVBO::pushBack(b2Vec2 pCenter, float angle, float radius, b2Vec3 color){
	//Get central point in pixels
	int moveX = pCenter.x * M2P;
	int moveY = pCenter.y * M2P;
	radius *= M2P;

	int numPoints = 60;
	float degreeStep = 360.0 / numPoints;

	//Creat points for circle (fan around center)
	for (float ang = degreeStep; ang <= 360; ang += degreeStep) {
		//Push back central point
		vertices.push_back(moveX);
		vertices.push_back(moveY);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);

		float thisX = cosf(ang) * radius;
		float thisY = sinf(ang) * radius;
		float pointX = moveX + thisX;
		float pointY = moveY + thisY;

		//Store 2nd point
		vertices.push_back(pointX);
		vertices.push_back(pointY);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);

		float lastX = cosf(ang - degreeStep) * radius;
		float lastY = sinf(ang - degreeStep) * radius;
		float lastPointX = moveX + lastX;
		float lastPointY = moveY + lastY;

		//Store 3rd point
		vertices.push_back(lastPointX);
		vertices.push_back(lastPointY);
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



