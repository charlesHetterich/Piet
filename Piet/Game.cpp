#include "Game.h"

int Animation::CANVAS_WIDTH = 2304;
int Animation::CANVAS_HEIGHT = 2304;

bool WorldSprite::rotateOnCollision = false;
float WorldSprite::destinationRotation = 0;
int WorldSprite::destinationQuadrant = 0;
float WorldSprite::cameraRotation = 0;
float WorldSprite::cameraFocusX = 0;
float WorldSprite::cameraFocusY = 0;
float WorldSprite::destinationFocusX = 0;
float WorldSprite::destinationFocusY = 0;
float WorldSprite::mouseX = 0;
float WorldSprite::mouseY = 0;

float* WorldStencilObject::cameraRotation = &WorldSprite::cameraRotation;
float* WorldStencilObject::cameraFocusX = &WorldSprite::cameraFocusX;
float* WorldStencilObject::cameraFocusY = &WorldSprite::cameraFocusY;

int Entity::GravityMultiplierX = 0;
int Entity::GravityMultiplierY = 0;

unsigned int Shader::CURRENT_SHADER = -1;

Game::Game()
{
	//shader
	shader[Shader::BASIC] = new BasicShader();
	shader[Shader::GRID] = new GridShader();
	shader[Shader::STENCIL] = new StencilShader();
	shader[Shader::WAVEY] = new WaveyShader();

	//texture
	spriteSheet = new CTexture("sprites.png");
	spriteSheet->bind(0);

	editor = LevelEditor(100, 100, shader[Shader::GRID], shader[Shader::BASIC], shader[Shader::WAVEY], shader[Shader::STENCIL]);


	std::vector<Vertex> positions;
	positions.push_back({ -500.0f, -500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ 500.0f, -500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ 500.0f, 500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ -500.0f, 500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });

	positions.push_back({ 600.0f, -500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ 1100.0f, -500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ 1100.0f, 500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });
	positions.push_back({ 600.0f, 500.0f, 0.0f, 1, 1, 1, 0.0f, 1.0f });

	stencil = WorldStencilObject(GL_EQUAL, shader[Shader::STENCIL], positions);
	box = WorldSprite(0, 0, 320, 320, shader[Shader::BASIC], Rect(0, 0, (float)Animation::CANVAS_WIDTH, (float)Animation::CANVAS_HEIGHT));
}

Game::~Game()
{
	delete [] &shader;
	delete spriteSheet;
}

void Game::input(Input input)
{
	//set mouse position in world
	WorldSprite::setMouse(input);

	//editor inputs
	editor.input(input);
}

void Game::update()
{
	WorldSprite::updateCameraFocus();
	WorldSprite::updateCameraRotation();
	editor.update();
}

void Game::render()
{
	editor.draw();
}