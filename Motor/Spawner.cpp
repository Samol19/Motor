#include "Spawner.h"
#include "Zombie.h"

Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}

void Spawner::update()
{
	if (timeForZombie <= 0) {
		
		timeForZombie = timeGeneration;
		spawn = true;
	}
	else {
		timeForZombie--;
	}
}

void Spawner::init(int timeGeneration, glm::vec2 position)
{
	this->position = position;
	color.set(0, 0, 0, 255);
	
	this->timeGeneration = timeGeneration;
	this->timeForZombie = timeGeneration;
	
	this->spawn = false;
}

void Spawner::checkSpawnZombie(vector<Zombie*>& zombies, float speed)
{
	if (spawn) {
		zombies.push_back(new Zombie);
		zombies.back()->init(speed, position);
		spawn = false;
	}
	
}
