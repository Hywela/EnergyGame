#include "RenderData.h"
#include "Platform.h"

RenderData::RenderData() {
}

RenderData::RenderData(int type) {
	this->type = type;
}
RenderData::RenderData(int type, b2Vec2 center, float angle, float radius, b2Vec3 color){
	this->type = type;

	this->center = center;
	this->angle = angle;
	this->radius = radius;
	this->color = color;

}
RenderData::RenderData(int type, b2Vec2 xy[4], b2Vec2 center, float angle, b2Vec3 color) {
	this->type = type;
	for (int i = 0; i < 4; i++) {
		points[i] =xy[i];
	}
	this->center = center; 
	this->angle = angle; 
	this->color = color;

}
void RenderData::test(){
	Platform form;
	
	form.draw(points, center, angle, color);

}
void RenderData::circle(){
	Circle form;

	form.draw(center, angle, radius, color);
}

RenderData::~RenderData() {

}

b2Vec2  RenderData::getCenter(){
	return center;
}
b2Vec2  RenderData::getXY(){
	return *points;
}
b2Vec3  RenderData::getColor(){
	return color;
}
float RenderData::getAngle(){

	return angle;
}
float RenderData::getRadius(){

	return radius;
}
int RenderData::getType(){
	return this->type;
}

int RenderData::getX(){
	return this->type;
}

int RenderData::getY(){
	return this->type;
}