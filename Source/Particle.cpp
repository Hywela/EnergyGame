#include "Particle.h"



Particle::Particle() {
	//Initialize pointers
	body = NULL;
	joint = NULL;

	//Set random color
	float rollR = randomRange(1, 100) / 100.0;
	float rollG = randomRange(1, 100) / 100.0;
	float rollB = randomRange(1, 100) / 100.0;
	color = b2Vec3(rollR, rollG, rollB);

	timeFired = -1;
}

Particle::~Particle() {

}

void Particle::setJoint(b2Joint *joint) {
	this->joint = joint;
	timeFired = 0;
}

b2Joint* Particle::fire() {
	b2Joint *temp = joint;

	joint = NULL;
	timeFired++;

	return temp;
}

void Particle::setBody(b2Body *body) {
	this->body = body;
}

b2Body* Particle::getBody() {
	return body;
}

b2Vec3 Particle::getColor() {
	return color;
}

bool Particle::onGround() {
	bool grounded = false;

	if ((timeFired == -1) || (timeFired >= 120)) {
		grounded = true;
	}

	return grounded;
}

bool Particle::isFired() {
	bool fired = false;

	if (timeFired > 0) {
		fired = true;
	}

	return fired;
}

void Particle::update() {
	if (isFired()) {
		timeFired++;
	}
}