#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human()
{
}

Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
	this->speed = speed;
	color.set(255, 255, 255, 255); 
	this->position = position;

	//Aletorizar primer frame del sprite e inicilizar variables
	srand(time(NULL));
	currentFrame = rand() % 4;
	valDirection = 1;
	animationSpeed = 0;

	////////////////Comportamientos/////////////
	// obtner direccion
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);
	direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction.length() == 0) {
		direction = glm::vec2(1.0f, 1.0f);
	}
	//direction = glm::vec2(direction);


	//Lista de Sprites
	vector<std::string> spritePaths = {
	"Images/1.png",
	"Images/2.png",
	"Images/3.png",
	"Images/4.png",
	"Images/5.png",
	"Images/6.png",
	"Images/7.png",
	"Images/8.png",
	"Images/9.png",
	"Images/10.png"
	};
	randomIndex = rand() % spritePaths.size();

	spritePath = spritePaths[randomIndex];
}

void Human::update(vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{

	// MOVIMIENTO ALEATORIO
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float>randRotate(-40.0f, 40.0f);
	position += direction * speed;

	//si coliciona se mueve aleatoriamente
	if (collideWithLevel(levelData)) {
		direction = glm::rotate(direction, randRotate(randomEngine));
	}

	// Genera un número aleatorio del 1 al 3
	int randomNum = rand() % 2 + 1;

	// Si el número es 1, cambia la dirección aleatoriamente
	if (randomNum == 1) {
		direction = glm::rotate(direction, glm::radians(randRotate(randomEngine)));
	}
}

void Human::draw()
{
	spritebatch.init();
	spritebatch.begin();
	//crear uvRect para rectangulo del frame de la imagen a dibujar
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f,1.0f);
	//crear destRect para posición del dibujo
	glm::vec4 destRect(position.x, position.y, AGENT_WIDTH * 0.9, AGENT_WIDTH * 0.8);
	spritebatch.draw(destRect, uvRect, ResourceManager::getTexture(spritePath).id, 0.0f, color);
	spritebatch.end();
	spritebatch.renderBatch();
}
