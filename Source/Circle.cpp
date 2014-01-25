#include "Circle.h"
int ground = 0;
const float M2P = 30;
const float P2M = 1 / M2P;
Circle::Circle(float x, float y, float radius, float r, float g, float b) {


	this->r = r;
	this->g = g;
	this->b = b;

	//SetPatrol(false);

	forcex = forcey = 0;
	distx = disty = 0;
	movex = movey = 0;

	this->radius = radius;

	
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
void Circle::draw( b2Vec2 center, float angle){
	vertices.clear();
	colors.clear();
	vertices.push_back(0);
	vertices.push_back(0);
	for (float i = 0.0; i <= 360; i += 360.0/30 ) {
		float thisX = (cos(i*M_PI / 180.0)* (radius))*M2P;
		float thisY = (sin(i*M_PI / 180.0)*(radius))*M2P;
		

		//Center
		vertices.push_back(thisX);
		vertices.push_back(thisY);
	
		
	}

	//glColor3f(1, 1, 1);
	glTranslatef(center.x*M2P, center.y*M2P, 0);
	glRotatef(angle*180.0 / M_PI, 0, 0, 1);

	


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
	
	

	//draw the Cube
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/2);

	//restore the state GL back
	
	glDisableClientState(GL_VERTEX_ARRAY);




	glTranslatef(-center.x*M2P, -center.y*M2P, 0);

}
void Circle::addCircle(int x, int y, int w, int h, bool dyn, b2World *world){
	this->x = x;
	this->y = y;
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	b2Body* body = world->CreateBody(&bodydef);

	b2CircleShape shape;
		
	shape.m_radius = radius;
	shape.m_p.Set(0, 0);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	fixturedef.friction = 0.0;
	body->CreateFixture(&fixturedef);
	body->SetUserData(&ground);
	
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


