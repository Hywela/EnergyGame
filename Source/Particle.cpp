#include "Particle.h"



Particle::Particle() {
	//Initialize pointers
	body = NULL;
	joint = NULL;

	//Set random color
	float totCol = 0;
	float rollR, rollG, rollB;
	while (totCol < 0.3) {
		//While dim color
		rollR = randomRange(1, 100) / 100.0;
		rollG = randomRange(1, 100) / 100.0;
		rollB = randomRange(1, 100) / 100.0;
		totCol = rollR + rollG + rollB;
	}
	color = b2Vec3(rollR, rollG, rollB);

	timeFired = -1;
}

Particle::~Particle() {
	if (body != NULL) {
		body = NULL;
	}

	if (joint != NULL) {
		joint = NULL;
	}

	timeFired = -1;
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

	if ((timeFired == -1) || (timeFired >= 60)) {
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