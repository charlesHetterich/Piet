#pragma once
#include "globalVariablesAndIncludes.h"
#include "Input.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Levels.h"
#include "StencilObject.h"

class Game
{
public:
	Game();
	~Game();

	void input(Input input);
	void update();
	void render();

private:

	LevelEditor editor;

	//testerooni
	WorldStencilObject stencil;
	WorldSprite box;

	//---------------------------------------------------------------------------
	//CORE
	//---------------------------------------------------------------------------
	//shaders
	Shader* shader[Shader::NUM_SHADERS];

	//textures
	CTexture* spriteSheet;
};

