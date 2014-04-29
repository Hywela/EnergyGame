#include "PlatformVBO.h"

void PlatformVBO::draw(){
	if (vertices.size() > 0){
	/*	setVBO();
		
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexPointer(2, GL_FLOAT, 0, 0);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, colorID);
		glColorPointer(3, GL_FLOAT, 0, 0);

		glDrawArrays(GL_QUADS, 0, vertices.size()/2);

		glDisableClientState(GL_COLOR_ARRAY);
		//restore the GL state back
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode
*/
	}
	
}
void PlatformVBO::drawTXT(){
	
	if (vertices.size() > 0){
		setVBO();

		glBindTexture(GL_TEXTURE_2D, bgTexture->GetTexture());
		glEnable(GL_TEXTURE_2D); 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[VBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) *indices.size(), &indices.front(), GL_STATIC_DRAW);

		glBindVertexArray(arrayObject);

		glDrawElements(renderMode, indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
		glBindVertexArray(NULL);
		indices.clear();
	}

}
void PlatformVBO::drawTexture(){
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	if (vertices.size() > 0){
		setVBO();
		glBindTexture(GL_TEXTURE_2D, platform->GetTexture());
		glEnable(GL_TEXTURE_2D);

	
		glBindVertexArray(arrayObject);

		glDrawElements(renderMode, indices.size() , GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);
		glBindVertexArray(NULL);	
		indices.clear();
	}

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

	//

	

	bgTexture = new Texture("./Texture/coke_label.bmp");
	platform = new Texture("./Texture/shiphull.bmp");
	// Give the image to OpenGL

	
	
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


	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO_TEXCORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size(), &texCoords.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(VBO_TEXCORD);
	glVertexAttribPointer(VBO_TEXCORD, 2, GL_FLOAT, GL_FALSE, NULL, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[VBO_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) *indices.size(), &indices.front(), GL_STATIC_DRAW);
	

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
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

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
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
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
	indices.clear();
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
