#include "Platform.h"


Platform::Platform()
{
}

Platform::~Platform()
{
}
b2Body * Platform::addRect(int x, int y, int w, int h, bool dyn, b2World *world)
{
	
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M, y*P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;
	else 
		bodydef.type = b2_staticBody;

	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M*w / 2, P2M*h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	fixturedef.friction = 0.0;
	fixturedef.restitution = 0.8;
	body->CreateFixture(&fixturedef);
	body->SetUserData(0);
	return body;
}
void Platform:: draw(b2Vec2* points, b2Vec2 center, float angle){
	vector<float> vertices;
	
	//glColor3f(1, 1, 1);
	glTranslatef(center.x*M2P, center.y*M2P, 0);
	glRotatef(angle*180.0 / M_PI, 0, 0, 1);
	
	for (int i = 0; i < 4; i++){
	
		vertices.push_back(points[i].x*M2P);
		vertices.push_back(points[i].y*M2P);}


	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &vertices.front());
//	glEnableClientState(GL_COLOR_ARRAY);
	//glColorPointer(2, GL_FLOAT, 0, &colors.front());

	//draw the Cube
	glDrawArrays(GL_QUADS, 0, vertices.size()/2);

	//restore the state GL back
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	glTranslatef(-center.x*M2P, -center.y*M2P, 0);

}