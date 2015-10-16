#pragma once
#include "globalVariablesAndIncludes.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Input.h"

class Animation
{
public:
	static int CANVAS_WIDTH;
	static int CANVAS_HEIGHT;

	Animation(int x = 0, int y = 0, int w = 0, int h = 0, int numFrames = 0, int frameTime = 0)
	{
		basePosX = x;
		basePosY = y;
		frameWidth = w;
		frameHeight = h;
		this->numFrames = numFrames;
		this->frameTime = frameTime;
		frameTimer = SDL_GetTicks();
	}
	bool shouldAnimate() { return SDL_GetTicks() > frameTimer + frameTime; }
	Rect getAnimation()
	{
		//checks for next frame
		if (SDL_GetTicks() > frameTimer + frameTime)
		{
			currentFrame++;
			if (currentFrame >= numFrames)
			{
				currentFrame = 0;
			}

			frameTimer = SDL_GetTicks();
		}

		return Rect((basePosX + frameWidth*currentFrame) / (float)CANVAS_WIDTH, basePosY / (float)CANVAS_HEIGHT, frameWidth / (float)CANVAS_WIDTH, frameHeight / (float)CANVAS_HEIGHT);
	}
	Rect getFirstAnimation()
	{
		return Rect((basePosX) / (float)CANVAS_WIDTH, basePosY / (float)CANVAS_HEIGHT, frameWidth / (float)CANVAS_WIDTH, frameHeight / (float)CANVAS_HEIGHT);
	}
	inline int getCurrentFrame() { return currentFrame; }

	//single cycle
	void cycleOnce()
	{
		oneCycleTime = SDL_GetTicks();
		currentFrame = 0;
		frameTimer = SDL_GetTicks();
	}
	inline bool getOneCycle()
	{
		return oneCycleTime + frameTime * numFrames > SDL_GetTicks();
	}

private:
	//position
	unsigned int basePosX;
	unsigned int basePosY;

	//frames
	unsigned int frameWidth;
	unsigned int frameHeight;
	unsigned int numFrames;

	//Timing
	unsigned int frameTime; //milliseconds
	unsigned int frameTimer;
	unsigned int currentFrame = 0;

	//flags
	int oneCycleTime;
};

class Sprite
{
public:
	Sprite(float x, float y, float w, float h, Shader* shader, Rect firstAnimation);
	Sprite(){}
	~Sprite();
	
	//core
	void draw(Animation* animation);
	void draw();
	void drawWave();
	virtual glm::mat4 getTransform();

	//characteristics
	void setColor(float r, float g, float b) { color = { r, g, b, color.a }; };
	void setAnimation(Rect animation)
	{
		mesh->setTexCoords(animation);
	}

	//transformations
	glm::vec4& getColor() { return color; }
	inline void setX(float x) { rect.x = x; }
	inline float getX() { return rect.x; }
	inline void setY(float y) { rect.y = y; }
	inline float getY() { return rect.y; }
	inline void setW(float w) { rect.w = w; }
	inline float getW() { return rect.w; }
	inline void setH(float h) { rect.h = h; }
	inline float getH() { return rect.h; }
	inline void setAlpha(float a) { color.a = a; }
	inline float getAlpha() { return color.a; }

	//rotation
	inline void setRotation(float rot) { rotation = rot; }
	inline float getRotation() { return rotation; }

	//collisions
	Rect getRect() { return rect; }

protected:

	//characteristics
	glm::vec4 color = { 1, 1, 1, 1 };
	float rotation;

	//transformations
	Rect rect;

	Shader* shader;
	std::shared_ptr<Mesh> mesh;
};

class WorldSprite : public Sprite
{
public:
	//constant worldly things
	static float cameraFocusX;
	static float cameraFocusY;
	static float destinationFocusX;
	static float destinationFocusY;
	static void setCameraFocusX(float x)
	{
		cameraFocusX = x;
		destinationFocusX = x;
	}
	static void setCameraFocusY(float y)
	{
		cameraFocusY = y;
		destinationFocusY = y;
	}
	static void updateCameraFocus()
	{
		cameraFocusX += (destinationFocusX - cameraFocusX) / 1000;
		cameraFocusY += (destinationFocusY - cameraFocusY) / 1000;
	}
	static bool rotateOnCollision;
	static float cameraRotation;
	static float destinationRotation;
	static int destinationQuadrant;
	static void setCameraRotation(float rot)
	{
		cameraRotation = rot;
		destinationRotation = rot;
	}
	static void updateCameraRotation()
	{
		cameraRotation += (destinationRotation - cameraRotation) / 1000;
		if (abs(destinationRotation - cameraRotation) < 0.0000000001)
			cameraRotation = destinationRotation;

		destinationQuadrant = (int(destinationRotation / 90) + 4000) % 4;
	}
	//mouse pos in world
	static float mouseX;
	static float mouseY;
	static void setMouse(Input input)
	{
		mouseX = (input.getMouseX() - ACTUAL_SCREEN_WIDTH / 2)*((2 * SCREEN_WIDTH) / ACTUAL_SCREEN_WIDTH) + WorldSprite::cameraFocusX;
		mouseY = (ACTUAL_SCREEN_HEIGHT / 2 - input.getMouseY())*((2 * SCREEN_HEIGHT) / ACTUAL_SCREEN_HEIGHT) + WorldSprite::cameraFocusY;
	}
	static float getMouseX()
	{
		return mouseX;
	}
	static float getMouseY()
	{
		return mouseY;
	}

	WorldSprite(float x, float y, float w, float h, Shader* shader, Rect firstAnimation) : Sprite(x, y, w, h, shader, firstAnimation){}
	WorldSprite(){}

	glm::mat4 getTransform()
	{
		return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::rotate(cameraRotation, glm::vec3(0, 0, 1)) * glm::translate(glm::vec3(rect.x + rect.w / 2 - (int)cameraFocusX, rect.y + rect.h / 2 - (int)cameraFocusY, 0)) * glm::scale(glm::vec3(rect.w, rect.h, 1)) * glm::rotate(rotation, glm::vec3(0, 0, 1));
	}
};
