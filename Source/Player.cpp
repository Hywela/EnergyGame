#include "Player.h"


Player::Player(){

}
Player::Player(float x, float y) {
	this->x = x;
	this->y = y;
	this->z = 0;

	fieldRadius = 40;
	fieldForce = 4;

	body = new Circle(x, y, 0.5f);
	
	//Get current time
    time_t timestamp = time(0);
    tm *now = new tm();
	localtime_s(now, &timestamp);

	//Randomize based of minutes and seconds
	srand((now->tm_min * 100) + now->tm_sec);

	//Spawn some orbs
	
}

Player::~Player() {

}
void Player::draw( b2Vec2 center, float angle){

	body->draw( center, angle);

}
void Player::Render() {
	int i = 0; //Particles drew

	//Draw half of the orbs behind
	for ( ; i < energyOrbs.size() / 2; i++) {
		energyOrbs[i]->Render();
	}

	//Draw player
	body->Render();

	//Draw rest of the orbs infront
	for ( ; i < energyOrbs.size(); i++) {
		energyOrbs[i]->Render();
	}
}

void Player::GetPosition(float &x, float &y) {
	x = this->x;
	y = this->y;
}

void Player::SpawnOrb() {
	//Place at random spot
	int rollx, rolly;
	PickRandomFieldSpot(rollx, rolly);
	energyOrbs.push_back(new Circle(x + rollx, y + rolly, 8, 0, 0, 1));

	//Move to random spot
	//PickRandomFieldSpot(rollx, rolly);
	energyOrbs.back()->MoveTo(x - rollx, y - rolly);
	energyOrbs.back()->SetPatrol(true);
}

void Player::Update() {
	//Update all orbs
	for (int i = 0; i < energyOrbs.size(); i++) {
		energyOrbs[i]->Update();

		//Set new target if still
		if (!energyOrbs[i]->IsMoving()) {
			int rollx, rolly;
			PickRandomFieldSpot(rollx, rolly);
			//energyOrbs[i]->MoveTo(x + rollx, y + rolly);
		}
	}
}

void Player::PickRandomFieldSpot(int &x, int &y) {
	x = (0 + (rand() % (int)fieldRadius) * (((rand() % 2) == 1) ? -1 : 1));
	y = (0 + (rand() % (int)fieldRadius) * (((rand() % 2) == 1) ? -1 : 1));
}

void Player::Keyboard(SDL_Event e) {
	switch (e.key.keysym.sym) {
		case SDLK_a: {
			//Move(-5, 0);
			break;
		}
		case SDLK_d: {
			//Move(5, 0);
			break;
		}
	}
}

void Player::Move(float speedx, float speedy) {
	x += speedx;
	y += speedy;
	body->Move(speedx, speedy);

	for (int i = 0; i < energyOrbs.size(); i++) {
		energyOrbs[i]->Move(speedx, speedy);
	}
}
void Player::addCircles(int x, int y, int w, int h, bool dyn, b2World *world){
	
	body->addCircle(x, y, w, h, dyn, world);
	

}