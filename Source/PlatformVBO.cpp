#include "PlatformVBO.h"

void PlatformVBO::draw(){
	if (vertices.size() > 0){
		setVBO();
		
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

	}

}
void PlatformVBO::drawTXT(){
	if (vertices.size() > 0){
		setVBO();

		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, texID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		if (platform->GetTexture() >= 0) {
			glEnable(GL_TEXTURE_2D);      // Turn on Texturing
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, platform->GetTexture());
		}



		glDrawArrays(GL_QUADS, 0, vertices.size() / 2);

		glDisableClientState(GL_COLOR_ARRAY);
		//restore the GL state back
		glDisableClientState(GL_VERTEX_ARRAY);
		if (platform->GetTexture() >= 0) {
			glDisable(GL_TEXTURE_2D);      // Turn off Texturing
			glBindTexture(GL_TEXTURE_2D, platform->GetTexture());
		}


		glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode

	}

}
void PlatformVBO::drawTexture(){


	if (vertices.size() > 0){
		setVBO();


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indID);

			//Enable states, and render (as if using vertex arrays directly)
			glEnableClientState(GL_VERTEX_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			glVertexPointer(2, GL_FLOAT, 0, 0);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, texID);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			if (bgTexture->GetTexture() >= 0) {
				glEnable(GL_TEXTURE_2D);      // Turn on Texturing
			//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glBindTexture(GL_TEXTURE_2D, bgTexture->GetTexture());
			}

			//Draw the thing!
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

			//restore the GL state back

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			if (bgTexture->GetTexture() >= 0) {
				glDisable(GL_TEXTURE_2D);      // Turn off Texturing
				glBindTexture(GL_TEXTURE_2D, bgTexture->GetTexture());
			}



			glBindBuffer(GL_ARRAY_BUFFER, 0); //Restore non VBO mode
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		
		
	}
}

PlatformVBO::PlatformVBO() {
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &colorID);
	glGenBuffers(1, &texID);
	glDeleteBuffers(1, &indID);
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);

	texCoords.push_back(1.0f);
	texCoords.push_back(0.0f);

	texCoords.push_back(1.0f);
	texCoords.push_back(1.0f);

	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f);

	//
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	

	bgTexture = new Texture("./Texture/shiphull.bmp");
	platform = new Texture("./Texture/coke_label.bmp");
	// Give the image to OpenGL


	glBindBuffer(GL_ARRAY_BUFFER, texID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size() / 2, &texCoords, GL_STATIC_DRAW);


	//glBindBuffer(GL_ARRAY_BUFFER, texID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *texCoords.size() / 2, &texCoords, GL_STATIC_DRAW);


}
PlatformVBO::~PlatformVBO(){
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &colorID);
	delete bgTexture;
	glDeleteBuffers(1, &texID);
	glDeleteBuffers(1, &indID);
}

void PlatformVBO::setVBO(){
	// Vertices:
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *vertices.size(), &vertices.front(), GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *colors.size(), &colors.front(), GL_STATIC_DRAW);



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
