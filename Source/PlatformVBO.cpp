#include "PlatformVBO.h"

void PlatformVBO::draw(bool texture){
	if (vertices.size() > 0)
		setVBO();

		setVBO();
		if (texture){
		glBindTexture(GL_TEXTURE_2D, bgTexture->GetTexture());
		glEnable(GL_TEXTURE_2D);

		}

		glBindVertexArray(arrayObject);

		glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

		if (texture){

		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
		}
		glBindVertexArray(NULL);
	
}
void PlatformVBO::drawTXT(){
	
	if (vertices.size() > 0){

		setVBO();

		
		glBindTexture(GL_TEXTURE_2D, bgTexture->GetTexture());
		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);


		glBindVertexArray(arrayObject);



		glDrawArrays(GL_QUADS, 0, vertices.size() / 2);
		glEnableClientState(GL_VERTEX_ARRAY);


		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
		glBindVertexArray(NULL);
	}

}
void PlatformVBO::drawTexture(){

	if(vertices.size() > 0)
		setVBO();
	glBindVertexArray(arrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_TEXCORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoordsPlat.size(), &texCoordsPlat.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(VBO_TEXCORD);
	glVertexAttribPointer(VBO_TEXCORD, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindVertexArray(NULL);

		glBindTexture(GL_TEXTURE_2D, platform->GetTexture());
		glEnable(GL_TEXTURE_2D);


		glBindVertexArray(arrayObject);
		glDrawArrays(GL_QUADS, 0, vertices.size() / 2);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
	
	glBindVertexArray(NULL);

}

PlatformVBO::PlatformVBO() {
	glGenVertexArrays(1, &arrayObject);
	glBindVertexArray(arrayObject);
	glGenBuffers(VBO_COUNT, buffers);
	renderMode = GL_TRIANGLES;
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);

	texCoords.push_back(1.0f);
	texCoords.push_back(0.0f);

	texCoords.push_back(1.0f);
	texCoords.push_back(1.0f);

	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f);
	
	bgTexture = new Texture("./Texture/shiphull.bmp");
	platform = new Texture("./Texture/motherboardEdit.bmp");

	//glBindBuffer(GL_ARRAY_BUFFER, texID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size() / 2, &texCoords, GL_STATIC_DRAW);


}
PlatformVBO::~PlatformVBO(){
	glDeleteBuffers(VBO_COUNT, buffers);
//	delete bgTexture;


}

void PlatformVBO::setVBO(){
	// Vertices:
	glBindVertexArray(arrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(VBO_VERTEX);
	glVertexAttribPointer(VBO_VERTEX, 2, GL_FLOAT, GL_FALSE, NULL, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors.front(), GL_STATIC_DRAW);

	//Set shader attribute data
	glEnableVertexAttribArray(VBO_COLOR);
	glVertexAttribPointer(VBO_COLOR, 3, GL_FLOAT, GL_FALSE, NULL, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_TEXCORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size(), &texCoords.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(VBO_TEXCORD);
	glVertexAttribPointer(VBO_TEXCORD, 2, GL_FLOAT, GL_FALSE, NULL, NULL);
	


	glBindVertexArray(NULL);
}
void PlatformVBO::pushBack(b2Vec2* points, b2Vec2 center, float angle, b2Vec3 color){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	
	for (int i = 0; i < 4; i++) {
		//std::cout << "c " << center.x << " " << points[i].x * M2P << " d " << points[i].y * M2P;
		vertices.push_back(points[i].x  *M2P + center.x*M2P);
		vertices.push_back(points[i].y *M2P + center.y*M2P);
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
		
	}
	texCoordsPlat.push_back(0.0f);
	texCoordsPlat.push_back(0.0f);

	texCoordsPlat.push_back(1.0f);
	texCoordsPlat.push_back(0.0f);

	texCoordsPlat.push_back(1.0f);
	texCoordsPlat.push_back(1.0f);

	texCoordsPlat.push_back(0.0f);
	texCoordsPlat.push_back(1.0f);

}
void PlatformVBO::pushBackground(b2Vec2* points, b2Vec2 center,  b2Vec3 color){

	for (int i = 0; i < 4; i++) {
		//std::cout << "c " << center.x << " " << points[i].x * M2P << " d " << points[i].y * M2P;
		vertices.push_back(points[i].x );
		vertices.push_back(points[i].y );
		colors.push_back(color.x);
		colors.push_back(color.y);
		colors.push_back(color.z);
	
	}

}
void PlatformVBO::pushBackLigthPostionLit(b2Vec2 center){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	litPostions.push_back(moveX);
	litPostions.push_back(moveY);
}
void PlatformVBO::pushBackLigthPostionUnlit(b2Vec2 center){
	int moveX = center.x*M2P;
	int moveY = center.y*M2P;
	unlitPostions.push_back(moveX);
	unlitPostions.push_back(moveY);
}

void PlatformVBO::clear(){
	vertices.clear();
	colors.clear();
	texCoordsPlat.clear();
	if (litPostions.size() > 0)
	litPostions.clear();
	if (unlitPostions.size() > 0)
	unlitPostions.clear();
}

GLfloat *PlatformVBO::getCenterUnlit( ){


	int s = unlitPostions.size();
	GLfloat *temp;
	//std::cout << " " << s;
	temp = new GLfloat[s];
	for (int i = 0; i < s; i += 1){
		temp[i] = unlitPostions.at(i);
	}
	
	return temp;
}
GLfloat *PlatformVBO::getCenterLit(){


int s = litPostions.size();
GLfloat *temp;
	//std::cout << " " << s;
	temp = new GLfloat[s];
	for (int i = 0; i < s; i += 1){
		temp[i] = litPostions.at(i);
	}

	return temp;
}
int PlatformVBO::getCenterLitSize(){


	return litPostions.size()/2;
}
int PlatformVBO::getCenterUnlitSize(){


	return unlitPostions.size()/2;
}
