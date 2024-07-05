#include "Zombie.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include "Human.h"


Zombie::Zombie()
{
	
}

Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
	srand(time(NULL));

	//areatolizar primer frame
	currentFrame = rand() % 4;

	//valor por defecto
	valDirection = 1;
	animationSpeed = 0;

	this->speed = speed;
	color.set(255, 255, 255, 255);
	this->position = position;
	this->furia_duration = 0;

	// obtner direccion aleatoria
	
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);
	direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction.length() == 0) {
		direction = glm::vec2(1.0f, 1.0f);
	}
}

void Zombie::update(vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	collideWithLevel(levelData);
	Human* closeHuman = getNearestHuman(humans);
	if (closeHuman != nullptr) {
		glm::vec2 direction = glm::normalize(
			closeHuman->getPosition() - position
		);
		position += direction * speed;

		//cambiar validor de dirección para el Sprite (Mirar izquierda o derecha)
		if ((direction.x * speed) < 0) {
			valDirection = -1;
		}
		if ((direction.x * speed) >= 0) {
			valDirection = 1;
		}

	}
	//Reduccion de tiempo entre frames del sprite
	animationSpeed++;
	if (animationSpeed == 8) {
		currentFrame = (currentFrame + 1) % 4;
		animationSpeed = 0;
	}



}

Human* Zombie::getNearestHuman(vector<Human*>& humans)
{
	Human* closeHuman = nullptr;
	float smallestDistance = 888888.0f;
	for (size_t i = 0; i < humans.size(); i++)
	{
		glm::vec2 dist = humans[i]->getPosition() - position;
		float distance = glm::length(dist);
		if (distance < smallestDistance) {
			smallestDistance = distance;
			closeHuman = humans[i];
		}
	}
	return closeHuman;
}

void Zombie::draw()
{
	spritebatch.init();
	spritebatch.begin();


	// Calcular uvRect para la animación
	glm::vec4 uvRect(currentFrame * (1.0f / 4), 0.0f, valDirection * (1.0f / 4), 1.0f);
	// Calcular uvRect de la posicion
	glm::vec4 destRect(position.x, position.y, (AGENT_WIDTH) * 1.3, AGENT_WIDTH * 1.2);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture("Images/zombie.png").id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();


}
