#pragma once

#include "globalVariablesAndIncludes.h"
#include "Tiles.h"
#include "Entities.h"
#include "StencilObject.h"

//editor animations
//blocks
enum
{
	RED_BLOCK,
	YELLOW_BLOCK,
	BLUE_BLOCK,

	NUM_BLOCK_TYPES
};
static Animation blockAnimations[NUM_BLOCK_TYPES] =
{
	Animation(4, 1, 1, 1, 0, 1000),		//red
	Animation(7, 1, 1, 1, 0, 1000),		//yellow
	Animation(10, 1, 1, 1, 0, 1000),	//blue
};

//backgrounds & normal map image
enum
{
	SPACE_BACKGROUND,
	SPOUT_BACKGROUND,
	WHITE_BACKGROUND,
	NORMALMAP_FOREGROUND,

	NUM_BACKGROUNDS
};
static Animation backgroundAnimations[NUM_BACKGROUNDS] =
{
	Animation(432, 0, 792, 432, 0, 1000),	//space
	Animation(224, 0, 198, 108, 0, 1000),	//spout
	Animation(1, 1, 1, 1, 1, 1000),			//white
	Animation(1224, 0, 792, 432, 0, 1000),			//normal map foreground
};

//tile
enum
{
	CLEAR_TILE,
	SPACE_TILE,

	NUM_TILE_TYPES
};
static Animation tileAnimations[NUM_TILE_TYPES] =
{
	Animation(1, 49, 14, 14, 1, 10000),		//space tile
	Animation(1, 65, 14, 14, 1, 10000),		//clear tile
};

class Level
{
public:
	Level(Shader* shader, Shader* waveShader, Shader* stencilShader) :
					shader(shader), waveShader(waveShader), stencilShader(stencilShader),
					player(shader),
					space(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, backgroundAnimations[SPACE_BACKGROUND].getFirstAnimation()),
					redSpace(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, backgroundAnimations[WHITE_BACKGROUND].getFirstAnimation()),
					blueSpace(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, backgroundAnimations[WHITE_BACKGROUND].getFirstAnimation()),
					background(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, backgroundAnimations[SPOUT_BACKGROUND].getFirstAnimation()),
					normalMapImage(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, shader, backgroundAnimations[NORMALMAP_FOREGROUND].getFirstAnimation()),
					timeSpeed(1), timer(0)
	{
		blueSpace.setColor(0, 0, 1);
		blueSpace.setAlpha(0.5);
		redSpace.setColor(1, 0, 0);
		redSpace.setAlpha(0.5);
		normalMapImage.setAlpha(0.1);
	}
	Level(){}
	
	//building the level
	void load(std::ifstream data)
	{
		//clear world
		clearWorld();

		//create blocks
		int numBlocks;
		data >> numBlocks;
		for (int i = 0; i < numBlocks; i++)
		{
			//get dimensions
			float x, y, w, h, type;
			data >> x;
			data >> y;
			data >> w;
			data >> h;
			data >> type;

			//get time
			//<--- TODO --->//

			//create block
			if (type == 0)
				blocks.push_back(std::shared_ptr<Block>(new RedBlock(x, y, w, h, shader)));
			else if (type == 1)
				blocks.push_back(std::shared_ptr<Block>(new YellowBlock(x, y, w, h, shader)));
			else if (type == 2)
				blocks.push_back(std::shared_ptr<Block>(new BlueBlock(x, y, w, h, waveShader)));
		}

		//create grid and stencil
		int w, h;
		std::vector<Vertex> positions;
		data >> w;
		data >> h;
		grid.resize(w);
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				int val;
				data >> val;
				grid[x].push_back(val);

				if (val != -2)
				{
					positions.push_back({ (x) * tileWidth, (y + 1) * tileHeight, 0, 0, 0, 0, 0, 0 });
					positions.push_back({ (x + 1)* tileWidth, (y + 1)* tileHeight, 0, 0, 0, 0, 0, 0 });
					positions.push_back({ (x + 1)* tileWidth, (y)* tileHeight, 0, 0, 0, 0, 0, 0 });
					positions.push_back({ (x)* tileWidth, (y)* tileHeight, 0, 0, 0, 0, 0, 0 });
				}
			}
		}
		stencil[0] = WorldStencilObject(GL_NOTEQUAL, stencilShader, positions);
		stencil[1] = WorldStencilObject(GL_NOTEQUAL, stencilShader, positions);
		stencil[2] = WorldStencilObject(GL_NOTEQUAL, stencilShader, positions);

		//set player position
		float x, y;
		data >> x;
		data >> y;
		player.setOriginalX(x);
		player.setOriginalY(y);

		//close things
		data.close();

		//reset things
		resetWorld();
	}

	void input(Input input)
	{
		doInput(input);
	}
	void doInput(Input input)
	{
		if (input.getKeyDown(SDL_SCANCODE_SPACE))
		{
			WorldSprite::destinationRotation = 0;
		}

		player.input(input);
	}
	void draw()
	{
		//draw background
		background.draw();

		//draw player
		player.draw();

		//draw blocks
		for (unsigned int i = 0; i < blocks.size(); i++)
			blocks[i]->draw();

		//draw other entities
		for (unsigned int i = 0; i < entities.size(); i++)
			entities[i]->draw();

		//draw space
		stencil[2].bind();
		blueSpace.draw();
		stencil[2].unbind();
		stencil[1].bind();
		redSpace.draw();
		stencil[1].unbind();
		stencil[0].bind();
		space.draw();
		stencil[0].unbind();

		//draw normal map
		normalMapImage.draw();
	}
	void update()
	{
		static float lastDestinationRotation = WorldSprite::destinationRotation;
		static float rotTimer = SDL_GetTicks();

		if (lastDestinationRotation != WorldSprite::destinationRotation)
		{
			lastDestinationRotation = WorldSprite::destinationRotation;
			rotTimer = (float)SDL_GetTicks();
		}
		else if (rotTimer + 500 > SDL_GetTicks())
			if (timeSpeed > 0.3)
				timeSpeed -= 0.0003;
			else
				timeSpeed = 0.3;
		else if (rotTimer + 500 < SDL_GetTicks())
			if (timeSpeed < 1)
				timeSpeed += 0.0003;
			else
				timeSpeed = 1;
			
		if (timer >= 1)
		{
			doUpdate();
			timer -= 1;
		}
		timer += timeSpeed;
	}
	void doUpdate()
	{
		if (WorldSprite::destinationRotation == 0)
		{
			Entity::GravityMultiplierX = 0;
			Entity::GravityMultiplierY = 1;
		}
		else if (WorldSprite::destinationRotation == 90)
		{
			Entity::GravityMultiplierX = 1;
			Entity::GravityMultiplierY = 0;
		}
		else if (WorldSprite::destinationRotation == 180)
		{
			Entity::GravityMultiplierX = 0;
			Entity::GravityMultiplierY = -1;
		}
		else if (WorldSprite::destinationRotation == 270)
		{
			Entity::GravityMultiplierX = -1;
			Entity::GravityMultiplierY = 0;
		}
		waveShader->updateClock();
		move();
	}
	void move()
	{
		//move player
		player.move();

		//collisions
		Rect rect = player.getImage()->getRect();
		std::vector<int> collidingIndex;
		for (int x = rect.x / tileWidth; x <= (rect.x + rect.w) / tileWidth; x++)
			for (int y = rect.y / tileHeight; y <= (rect.y + rect.h) / tileHeight; y++)
			{
				if (grid[x][y] == -2)
				{
					resetWorld();
					x = 100000;
					y = 100000;
					collidingIndex.clear();
				}
				else if (grid[x][y] >= 0)
				{
					bool shouldAdd = true;

					for (unsigned int i = 0; i < collidingIndex.size(); i++)
						if (collidingIndex[i] == grid[x][y])
							shouldAdd = false;
					
					if (shouldAdd)
						collidingIndex.push_back(grid[x][y]);
				}
			}
		for (unsigned int i = 0; i < collidingIndex.size(); i++)
			blocks[collidingIndex[i]]->playerCollision(&player);

		//shake space
		stencil[2].setX(stencil[1].getX());
		stencil[2].setY(stencil[1].getY());
		stencil[1].setX(stencil[0].getX());
		stencil[1].setY(stencil[0].getY());
		stencil[0].setX((float)(rand() % 10 - 5));
		stencil[0].setY((float)(rand() % 10 - 5));

		//set camera focus
		WorldSprite::destinationFocusX = player.getImage()->getX();
		WorldSprite::destinationFocusY = player.getImage()->getY();
	}

private:

	//speed of the world
	float timeSpeed;
	float timer;

	//reset the world
	void clearWorld()
	{
		blocks.clear();
		entities.clear();
		grid.clear();
	}
	void resetWorld()
	{
		WorldSprite::destinationRotation = 0;
		player.reset();
	}

	//data
	std::vector<std::vector<int>> grid;

	//objects
	Player player;
	std::vector<std::shared_ptr<Block>> blocks;
	std::vector<std::shared_ptr<Entity>> entities;

	//backgrounds
	Sprite space;
	Sprite redSpace;
	Sprite blueSpace;
	Sprite background;

	//giving the screen a texture
	Sprite normalMapImage;

	//stencil
	WorldStencilObject stencil[3];

	//info
	float tileWidth = 200.0f;
	float tileHeight = 200.0f;

	//shader
	Shader* shader;
	Shader* waveShader;
	Shader* stencilShader;
};

class String
{
public:
	String();

private:

	std::vector<Level> levels;
};

class Universe
{
public:
	Universe();

private:
	std::vector<String> strings;
};



struct EditorObject
{
public:
	EditorObject(int type, float x, float y, float w, float h, Shader* shader, Rect firstAnimation)
	{
		this->type = type;
		image = WorldSprite(x, y, w, h, shader, firstAnimation);
	}
	int type;
	WorldSprite image;
};

class LevelEditor
{
public:
	LevelEditor(){}
	LevelEditor(int w, int h, Shader* tileShader, Shader* basicShader, Shader* waveShader, Shader* stencilShader) :
		shader(basicShader),
		width(w), height(h),
		tile(w, std::vector<std::shared_ptr<WorldSprite>>(h)),
		editing(true),
		selector(0, 0, tileWidth, tileHeight, basicShader, selectorAnimation.getFirstAnimation()),
		xMin(0), rootX(0), yMin(0), rootY(0),
		gridData(w, std::vector<int>(h, -2)),
		player(0, 0, 150, 150, basicShader, Rect(0, 0, 1, 1))
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				tile[x][y].reset(new WorldSprite(tileWidth * x, tileHeight * y, tileWidth, tileHeight, basicShader, tileAnimations[SPACE_TILE].getFirstAnimation()));
			}
		}
		level = Level(basicShader, waveShader, stencilShader);

		//selector
		selector.setAlpha(0.1);
	}

	void draw()
	{
		if (editing)
			drawEditor();
		else
			drawLevel();
	}
	void input(Input input)
	{
		if (editing)
			editorInput(input);
		else
			levelInput(input);
	}
	void update()
	{
		if (editing)
			updateEditor();
		else
			updateLevel();
	}
private:

	//drawing
	void drawLevel()
	{
		level.draw();
	}
	void drawEditor()
	{
		//draw blocks
		for (unsigned int i = 0; i < blocks.size(); i++)
			blocks[i].image.draw();

		//draw entities
		for (unsigned int i = 0; i < entities.size(); i++)
			entities[i].image.draw();

		//draw player image
		player.draw();

		//draw grid
		for (int y = int((WorldSprite::cameraFocusY - SCREEN_HEIGHT) / tileHeight); y < (WorldSprite::cameraFocusY + SCREEN_HEIGHT) / tileHeight; y++)
		{
			for (int x = int((WorldSprite::cameraFocusX - SCREEN_WIDTH) / tileWidth); x < (WorldSprite::cameraFocusX + SCREEN_WIDTH) / tileWidth; x++)
			{
				if (y >= 0 && x >= 0 && y < height && x < width)
					tile[x][y]->draw();
			}
		}

		//draw selector
		selector.draw();
	}

	//inputs
	void editorInput(Input input)
	{
		//move across screen
		if (input.getKey(SDL_SCANCODE_W))
			WorldSprite::destinationFocusY += 0.3f;
		if (input.getKey(SDL_SCANCODE_A))
			WorldSprite::destinationFocusX -= 0.3f;
		if (input.getKey(SDL_SCANCODE_S))
			WorldSprite::destinationFocusY -= 0.3f;
		if (input.getKey(SDL_SCANCODE_D))
			WorldSprite::destinationFocusX += 0.3f;
		//set selector roots
		if (!input.getMouseButton(2))
		{
			rootX = int(WorldSprite::getMouseX() / tileWidth);
			rootY = int(WorldSprite::getMouseY() / tileHeight);
		}

		//setting proccess
		int lastProccess = currentProccess;
		if (input.getKeyDown(SDL_SCANCODE_1))
			currentProccess = 0;
		else if (input.getKeyDown(SDL_SCANCODE_2))
			currentProccess = 1;
		else if (input.getKeyDown(SDL_SCANCODE_3))
			currentProccess = 2;
		else if (input.getKeyDown(SDL_SCANCODE_4))
			currentProccess = 3;
		if (currentProccess != lastProccess)
			currentType = 0;
		//doing current proccess
		if (currentProccess == 0)
			editSpace(input);
		else if (currentProccess == 1)
			editPlayer(input);
		else if (currentProccess == 2)
			editBlocks(input);
		else if (currentProccess == 3)
			editEntities(input);

		//toggle editing & playing
		if (input.getKeyDown(SDL_SCANCODE_F1))
			saveAndReload();
	}
	void editSpace(Input input)
	{
		//set type
		if (input.getKeyDown(SDL_SCANCODE_Z))
			currentType = 0;
		else if (input.getKeyDown(SDL_SCANCODE_X))
			currentType = 1;

		//set selector piture
		selector.setAnimation(tileAnimations[currentType].getFirstAnimation());

		//edit space
		if (input.getMouseButtonDown(0))
		{
			for (int x = xMin; x < xMin + selector.getW() / tileWidth; x++)
			{
				for (int y = yMin; y < yMin + selector.getH() / tileHeight; y++)
				{
					gridData[x][y] = -1 - currentType;
					tile[x][y]->setAnimation(tileAnimations[currentType].getFirstAnimation());
				}
			}
		}
	}
	void editPlayer(Input input)
	{
		if (input.getMouseButton(0))
		{
			player.setX(WorldSprite::getMouseX());
			player.setY(WorldSprite::getMouseY());
		}
	}
	void editBlocks(Input input)
	{
		//set type
		if (input.getKeyDown(SDL_SCANCODE_R))
			currentType = 0;
		else if (input.getKeyDown(SDL_SCANCODE_Y))
			currentType = 1;
		else if (input.getKeyDown(SDL_SCANCODE_B))
			currentType = 2;

		//set selector picture
		selector.setAnimation(blockAnimations[currentType].getFirstAnimation());

		//place object
		if (input.getMouseButtonDown(0))
		{
			for (int x = xMin; x < xMin + selector.getW() / tileWidth; x++)
			{
				for (int y = yMin; y < yMin + selector.getH() / tileHeight; y++)
				{
					gridData[x][y] = blocks.size();
				}
			}

			blocks.push_back(EditorObject(currentType, selector.getX(), selector.getY(), selector.getW(), selector.getH(), shader, blockAnimations[currentType].getFirstAnimation()));
		}
	}
	void editEntities(Input input){}	//<--- TODO --->//
	void levelInput(Input input)
	{
		//level inputs
		level.input(input);

		//toggle editing & playing
		if (input.getKeyDown(SDL_SCANCODE_F1))
			editing = true;
	}


	//update
	void updateEditor()
	{
		int selectedX = int(WorldSprite::getMouseX() / tileWidth);
		int selectedY = int(WorldSprite::getMouseY() / tileHeight);

		if (rootX < selectedX)
			xMin = rootX;
		else
			xMin = selectedX;

		if (rootY < selectedY)
			yMin = rootY;
		else
			yMin = selectedY;

		selector.setX(xMin * tileWidth);
		selector.setY(yMin * tileWidth);
		selector.setW((abs(selectedX - rootX) + 1) * tileWidth);
		selector.setH((abs(selectedY - rootY) + 1) * tileHeight);
	}
	void updateLevel()
	{
		level.update();
	}

	//save and re-load level
	void saveAndReload()
	{
		//save level
		std::ofstream outputData("res/data/level.txt");

		//save blocks
		outputData << blocks.size() << " ";
		for (unsigned int i = 0; i < blocks.size(); i++)
		{
			//save block dimensions
			outputData << blocks[i].image.getX() << " ";
			outputData << blocks[i].image.getY() << " ";
			outputData << blocks[i].image.getW() << " ";
			outputData << blocks[i].image.getH() << " ";
			outputData << blocks[i].type << " ";
		}

		//save grid data
		outputData << gridData.size() << " " << gridData[0].size() << " ";
		for (unsigned int y = 0; y < gridData[0].size(); y++)
			for (unsigned int x = 0; x < gridData.size(); x++)
				outputData << gridData[x][y] << " ";

		//save player position
		outputData << player.getX() << " " << player.getY() << " ";

		outputData.close();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//load level
		level.load(std::ifstream("res/data/level.txt"));

		//switch to level
		editing = false;
	}

	//world demensions
	int width;
	int height;

	//the level
	Level level;

	//grid
	std::vector<std::vector<std::shared_ptr<WorldSprite>>> tile;
	float tileWidth = 200.0f;
	float tileHeight = 200.0f;

	//world data
	std::vector<EditorObject> blocks;
	std::vector<EditorObject> entities;
	std::vector<std::vector<int>> gridData;
	WorldSprite player;

	//selector
	WorldSprite selector;
	Animation selectorAnimation = Animation(1, 1, 1, 1, 0, 1000);
	int xMin, rootX, yMin, rootY;

	//selected item hierarchy
	int currentProccess = 0; //clearing space, adding players pos, adding blocks, or adding entities
	int currentType = 0; //space clearing, blocks, and entities have their own list of types

	//flags
	bool editing;

	Shader* shader;
};