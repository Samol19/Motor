#include "MainGame.h"
#include "Sprite.h"

MainGame::MainGame()
{
	window = NULL;
	width = 800;
	height = 600;
	gameState = GameState::PLAY;

}

void MainGame::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("TA01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	//es valida si hubo un error
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		//falta validar el estado del glew
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	initShaders();

}

MainGame::~MainGame()
{


}

void MainGame::run()
{
	init();
	sprite.init(-1,-1,1,1);

	//INICIALIZAR PROPIEDADES DE LOS SPRITES(RECTANGULOS) A MOSTRAR
	sprite1.init(-1, 0, 1, 1);
	sprite2.init(0, 0, 1, 1);
	sprite3.init(0, -1, 1, 1);



	update();
}

void MainGame::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	GLuint timeLocation = program.getUniformLocation("time");
	glUniform1f(timeLocation, time);
	time += 0.008;
	
	sprite.draw();


	//DIBUJAR RECTANGULOS CON EL TIEMPO
	if (int(time) > 2) {
		sprite1.draw();
	}

	if (int(time) > 4) {
		sprite2.draw();
	}

	if (int(time) > 6) {
		sprite3.draw();
	}


	program.unuse();
	SDL_GL_SwapWindow(window);

}

void MainGame::update()
{
	while (gameState != GameState::EXIT) {
		draw();
		processInput();
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type) {
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			cout << " Posicion en X " << event.motion.x 
			<< " Posicion en Y " << event.motion.y << endl;
			break;
		}
	}
}

void MainGame::initShaders()
{
	program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	program.addAtribute("vertexPoistion");
	program.addAtribute("vertexColor");

	program.linkShader();

}
