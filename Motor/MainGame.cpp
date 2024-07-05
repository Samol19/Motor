#include "MainGame.h"
#include <iostream>
#include "Error.h"
#include <random>
#include <ctime>

using namespace std;

MainGame::MainGame() {
	timeconter = 0;
	timeconter2 = 0;
	width = 800;
	height = 600;
	gameState = GameState::PLAY;
	camera2D.init(width, height);
}

MainGame::~MainGame() {

}
void MainGame::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			/*cout << "Posicion del mousec " << event.motion.x << " " << event.motion.y << endl;
			inputManager.setMouseCoords(event.motion.x, event.motion.y);
			glm::vec2 mouseCoords = camera2D.convertToScreenWorld(inputManager.getMouseCoords());
			//cout << "Nueva posicion de acuerdo a camara " <<  mouseCoords.x
				//	<< " " << mouseCoords.y << endl;*/
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(event.key.keysym.sym);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(event.button.button);
			break;
		}
		if (event.type == SDL_MOUSEMOTION) {
			inputManager.setMouseCoords(event.motion.x, event.motion.y);
		}
		handleInput();
	}

}

void MainGame::handleInput()
{
	const float SCALE_SPEED = 0.01f;
	//TimeConter2 es un colddown del eliminador
	//Eliminar 1 sprite
	if (inputManager.isKeyDown(SDLK_e)) {
		if (!humans.empty()&&timeconter2%2==0) {
			delete humans.back();
			humans.pop_back();
		}
		timeconter2++;
	}
	//Eliminar 2 sprite
	if (inputManager.isKeyDown(SDLK_f)) {
		if (timeconter2 % 2 == 0) {
			if (!humans.empty()) {
				delete humans.back();
				humans.pop_back();
			}
			if (!humans.empty()) {
				delete humans.back();
				humans.pop_back();
			}}
		timeconter2++;

	}

	if (inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		//cout << "CLICK IZQUIERDO" << endl;

		glm::vec2 coords = camera2D.convertToScreenWorld(inputManager.getMouseCoords()); // coords relativas
		glm::vec2 direction = glm::normalize(coords - player->getPosition());
		createBullet(direction);

	}

	if (inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		//cout << "CLICK DERECHo" << endl;
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_MIDDLE)) {
		//cout << "CLICK CENTRO" << endl;
	}
}

void MainGame::createBullet(glm::vec2 direction) {
	if(!player->shotReady()) return;
	player->infoCD();
	// A mas amplitud de camara, mayor el coldown
	player->updateShotColdown(camera2D.getScale());
	player->resetCDShot();
	glm::vec2 pos = player->getPosition();
	Bullet *bullet = new Bullet();
	bullet->init(pos, direction, 8.0f);
	bullets.push_back(bullet);
	
}

void MainGame::initShaders()
{
	program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	program.addAtribute("vertexPosition");
	program.addAtribute("vertexColor");
	program.addAtribute("vertexUV");
	program.linkShader();
}

void MainGame::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window.create("Zoombies", width, height, 0);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Glew not initialized");
	}
	glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // RGB para color celeste claro
	initLevel(0);
	initShaders();
}

void MainGame::initLevel(int currentLevel) {
	spriteBatch.init();	
	levels.push_back(new Level("Level/level1.txt"));
	levels.push_back(new Level("Level/level2.txt"));
	levels.push_back(new Level("Level/level3.txt"));
	levels.push_back(new Level("Level/level4.txt"));
	levels.push_back(new Level("Level/level5.txt"));

	// init player
	player = new Player();
	float speed = 10.0f;
	float shotColdown = 10.0f;
	int lives = 3;
	player->init(speed, levels[currentLevel]->getPlayerPosition(),
		&inputManager, shotColdown, lives);

	
}

void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	GLuint pCameraLocation = program.getUniformLocation("pCamera");
	glUniformMatrix4fv(pCameraLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	GLuint imageLocation = program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);
	spriteBatch.begin();

	levels[currentLevel]->draw();
	for (size_t i = 0; i < humans.size(); i++)
	{
		humans[i]->draw();
	}
	for (auto &z : zombies)
	{
		z->draw();
	}
	for (auto &s : spawns) {
		s->draw();
	}
	for (auto& b : bullets) {
		b->draw();
	}
	player->draw();

	spriteBatch.end();
	spriteBatch.renderBatch();
	drawHud();
	glBindTexture(GL_TEXTURE_2D, 0);
	program.unuse();
	window.swapWindow();

	
}

void MainGame::drawHud()
{


}

void MainGame::run() {
	init();
	update();
}

void MainGame::updateElements() {
	
	camera2D.update();
	camera2D.setPosition(player->getPosition());
}

void MainGame::moveAndCollide() {
	///////////////////////// PLAYER //////////////////////////
	player->update(levels[currentLevel]->getLevelData(), humans, zombies);

	///////////////////////// HUMANS //////////////////////////
	// Movimiento y colisiones
	for (auto& h : humans)
	{
		h->update(levels[currentLevel]->getLevelData(), humans, zombies);
	}

	///////////////////////// ZOMBIES //////////////////////////
	//Movimiento e infecciones
	for (size_t i = 0; i < zombies.size(); i++)
	{
		zombies[i]->update(levels[currentLevel]->getLevelData(), humans, zombies);
		for (size_t j = 0; j < humans.size(); j++)
		{
			if (zombies[i]->collideWithAgent(humans[j])) {
				zombies.push_back(new Zombie);
				zombies.back()->init(2.0f, humans[j]->getPosition());
				delete humans[j];
				humans[j] = humans.back();
				humans.pop_back();

			}
		}
	}
	////////////////////////// SPAWNS //////////////////////////
	// Spawn de zombies con cooldown
	for (auto& s : spawns) {
		s->update();
		s->checkSpawnZombie(zombies, 2.0f);
	}
	///////////////////////// BULLETS //////////////////////////
	// Asesinatos y eliminacion de balas

	for (auto& b : bullets) {
		b->update();
	}
	for (size_t i = 0; i < zombies.size(); i++) // mejorable con Spatial Partitioning
	{
		for (size_t j = 0; j < bullets.size(); j++)
		{
			// asesinato y eliminacion de bala
			if (zombies[i]->collideWithAgent(bullets[j])) {

				delete zombies[i];

				zombies[i] = zombies.back();
				zombies.pop_back();
				delete bullets[j];
				bullets[j] = bullets.back();
				bullets.pop_back();
				break;
			}
			// eliminacion de bala al chocar con pared
			if (bullets[j]->iSForDestroy(levels[currentLevel]->getLevelData())) {
				delete bullets[j];
				bullets[j] = bullets.back();
				bullets.pop_back();
				break;
			}
		}
	}
}

void MainGame::update() {
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
		updateElements();
		moveAndCollide();// mejorable con Spatial Partitioning
		
		//CADA 3 SEGUNDOS CREAR UN SPRITE ALEATORIO
		//Si es menor a 20
		if(humans.size()<20)
			if (timeconter % 300 == 0) {
			humans.push_back(new Human());
			//crear sprite cerca del jugador
			humans.back()->init(0.5f, player->getPosition());
			}
		timeconter++;

		
		if (zombies.size() == 0) {
			passLevel();
		}
		for (auto &z : zombies) 
		{
			if (z->collideWithAgent(player)) {
				player->die();
				resetLevel();
				break;
			}
		}
		if (player->getLives() == 0) {
			reset();
			cout<<"GAME OVER"<<endl;
			gameState = GameState::EXIT;
		}

	}

}
// Reiniciar el juego al perder
void MainGame::reset() {

	glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // RGB para color celeste claro
	for (size_t i = 0; i < zombies.size(); i++)
	{
		delete zombies[i];
	}
	
	for (size_t i = 0; i < humans.size(); i++)
	{
		delete humans[i];
	}
	for (size_t i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}
	for (size_t i = 0; i < spawns.size(); i++)
	{
		delete spawns[i];
	}

	delete player;
	zombies.clear();
	humans.clear();
	levels.clear();
	spawns.clear();
	bullets.clear();
}

void MainGame::passLevel() {

}

void MainGame::resetLevel()
{
	reset();
	initLevel(currentLevel);
}

