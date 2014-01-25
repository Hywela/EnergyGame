#include "Sphere.h"



Sphere::Sphere(float x, float y, float z, float radius, float degreeStep, float heightStep) {
	this->x = x;
	this->y = y;
	this->z = z;

	this->radius = radius;

	int topCenter = -1;
	int bottomCenter = -1;
	int layer = 0;

	//Sphere:
	for (float ring = y - radius; ring <= y + radius; ring += heightStep) {
		float ringHeight = ring - y;
		float ringRadius = sqrt((radius * radius) - (ringHeight * ringHeight));

        if (ringRadius == 0) {
			int curx = x + (sin(0) * ringRadius);
			int curz = z + (cos(0) * ringRadius);

			//To or bottom: Add a single point
			for (int i = 0; i < 4; i++) {
				vertices.push_back(curx);
				vertices.push_back(ring);
				vertices.push_back(curz);
			}

			//Add normal as well
			//normals.push_back(vertices[vertices.size() - 3] * 2);
			//normals.push_back(vertices[vertices.size() - 2] * 2);
			//normals.push_back(vertices[vertices.size() - 1] * 2);

			topCenter = 0;
        }
		else {
			//For each degree in a circle
			int step = 0;
            for (float d = 0; d <= 360; d += degreeStep) {
				int stepCount = (360 / degreeStep);
				int topLeft = 12 + (stepCount * 12 * (layer - 2)) + (12 * step);
				int topRight = topLeft + 3;
				float curx = x + (sin(d) * ringRadius);
				float curz = z + (cos(d) * ringRadius);
				float nextx = x + (sin(d + degreeStep) * ringRadius);
				float nextz = z + (cos(d + degreeStep) * ringRadius);

				if (layer < 2) {
					topRight = topCenter;
					topLeft = topCenter;
				}
				else {
					cout << "gg";
				}

				//Top left
				vertices.push_back(vertices[topLeft]);
				vertices.push_back(vertices[topLeft + 1]);
				vertices.push_back(vertices[topLeft + 2]);

				//Top right
				vertices.push_back(vertices[topRight]);
				vertices.push_back(vertices[topRight + 1]);
				vertices.push_back(vertices[topRight + 2]);
				
				//Bottom right
				vertices.push_back(nextx);
				vertices.push_back(ring);
				vertices.push_back(nextz);

				//Bottom left
				vertices.push_back(curx);
				vertices.push_back(ring);
				vertices.push_back(curz);

				//Add normal as well
				//normals.push_back(vertices[vertices.size() - 3] * 2);
				//normals.push_back(vertices[vertices.size() - 2] * 2);
				//normals.push_back(vertices[vertices.size() - 1] * 2);
				step++;
            }
        }

		layer++;
	}
}

Sphere::~Sphere() {

}

void Sphere::Render() {
	//Render at position
	glTranslatef(x, y, z);

	//Tell OpenGL about our vertex and normal data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices.front());
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glNormalPointer(GL_FLOAT, 0, &normals.front());

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, vertices.size() / 3);

	//restore the state GL back
	//glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Move back to camera
	glTranslatef(-x, -y, -z);
}