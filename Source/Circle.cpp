#include "Circle.h"



Circle::Circle(float x, float y, float radius, float r, float g, float b) {
	this->x = x;
	this->y = y;
	this->z = 0;

	this->r = r;
	this->g = g;
	this->b = b;

	SetPatrol(false);

	forcex = forcey = 0;
	distx = disty = 0;
	movex = movey = 0;

	this->radius = radius;

	for (float d = 0; d < radius; d += 0.3) {
		float curx = x + (sin(d) * radius);
		float cury = y + (cos(d) * radius);

		//Center
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		colors.push_back(r);
		colors.push_back(g);
		colors.push_back(b);

		//Current
		vertices.push_back(curx);
		vertices.push_back(cury);
		vertices.push_back(z);
		colors.push_back(r * 0.5);
		colors.push_back(g * 0.5);
		colors.push_back(b * 0.5);
	}
}

Circle::~Circle() {
}

void Circle::Render() {
	//Render at position
	glTranslatef(x, y, z);

	//Tell OpenGL about our vertex and normal data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices.front());
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_QUAD_STRIP, 0, vertices.size() / 3);

	//restore the state GL back
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Move back to camera
	glTranslatef(-x, -y, -z);
}

void Circle::MoveDistance(int distx, int disty) {
	this->distx = distx;
	this->disty = disty;
}

void Circle::Update() {
	if (distx) {
		//Add new force
		x += forcex;
		distx -= forcex;

		//Make sure it's not over bounds
		if ((forcex < 0 && distx >= 0) || (forcex > 0 && distx <= 0)) {
			forcex = 0;
			distx = 0;
		}
		//Else add force if you can
		else if (forcex > -MAX_FORCE && forcex < MAX_FORCE) {
			forcex += ((distx > 0) ? 1 : -1);
		}
	}

	if (disty) {
		//Add new force
		y += forcey;
		disty -= forcey;

		//Make sure it's not over bounds
		if ((forcey < 0 && disty >= 0) || (forcey > 0 && disty <= 0)) {
			forcey = 0;
			disty = 0;
		}
		//Else add force if you can
		else if (forcey > -MAX_FORCE && forcey < MAX_FORCE) {
			forcey += ((disty > 0) ? 1 : -1);
		}
	}

	if (movex) {
		//x += forcex;
		x += ((movex > x) ? 1 : -1) * 5;

		//Make sure it's not over bounds
		if ((forcex < 0 && movex >= x) || (forcex > 0 && movex <= x)) {
			if (patrol) {
				forcex *= -1;
				movex = ((movex == pathxEnd) ? pathxStart : pathxEnd);
			}
			else {
				forcex = 0;
				movex = 0;
			}
		}
		//Else add force if you can
		else if (forcex > -MAX_FORCE && forcex < MAX_FORCE) {
			forcex += ((movex > x) ? 1 : -1);
		}
	}

	if (movey) {
		//y += forcey;
		y += ((movey > y) ? 1 : -1) * 5;

		//Make sure it's not over bounds
		if ((forcey < 0 && movey >= y) || (forcey > 0 && movey <= y)) {
			if (patrol) {
				forcey *= -1;
				movey = ((movey == pathyEnd) ? pathyStart : pathyEnd);
			}
			else {
				forcey = 0;
				movey = 0;
			}
		}
		//Else add force if you can
		else if (forcey > -MAX_FORCE && forcey < MAX_FORCE) {
			forcey += ((movey > y) ? 1 : -1);
		}
	}
}

bool Circle::HasTarget() {
	bool target = false;

	if (distx || disty) {
		target = true;
	}

	return target;
}

void Circle::MoveTo(int x, int y) {
	movex = x;
	movey = y;
	//forcex = ((movex > x) ? 1 : -1) * 2;
	//forcey = ((movey > y) ? 1 : -1) * 2;
}

bool Circle::IsMoving() {
	bool moving = false;

	if (movex || movey) {
		moving = true;
	}

	return moving;
}

void Circle::SetPatrol(bool state) {
	patrol = state;
	if (patrol) {
		pathxStart = x;
		pathyStart = y;
		pathxEnd = movex;
		pathyEnd = movey;
	}
	else {
		pathxStart = 0;
		pathyStart = 0;
		pathxEnd = 0;
		pathyEnd = 0;
	}
}

void Circle::Move(float speedx, float speedy) {
	if (patrol) {
		pathxStart += speedx;
		pathyStart += speedy;
		pathxEnd += speedx;
		pathyEnd += speedy;
	}
	else {
		x += speedx;
		y += speedy;
	}

	if (distx < 0) {distx -= speedx;}
	if (distx > 0) {distx += speedx;}
	if (disty < 0) {disty -= speedy;}
	if (disty > 0) {disty += speedy;}

	if (movex <= x) {movex -= speedx;}
	if (movex > x) {movex += speedx;}
	if (movey <= y) {movey -= speedy;}
	if (movey > y) {movey += speedy;}
}