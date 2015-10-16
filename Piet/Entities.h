#pragma once
#include "globalVariablesAndIncludes.h"

class Entity
{
public:

	//statics
	static int GravityMultiplierX;
	static int GravityMultiplierY;

	Entity(Shader* shader) : velocityX(0), velocityY(0), gravity(0), accelerationX(0), friction(0), landed(false)
	{
		this->shader = shader;
	}
	Entity(){}
	virtual void move()
	{
		setRelativeVelocityY(getRelativeVelocityY() - getGravity());

		image.setX(image.getX() + velocityX);
		image.setY(image.getY() + velocityY);

		landed = false;
	}
	virtual void draw(){}

	WorldSprite* getImage() { return &image; }

	//movement
	inline void setVelocityX(float velocity) { velocityX = velocity; }
	inline float getVelocityX() { return velocityX; }
	inline void setVelocityY(float velocity) { velocityY = velocity; }
	inline float getVelocityY() { return velocityY; }
	inline float getFriction() { return friction; }
	inline float getGravity() { return gravity; }
	float getGravityX()
	{
		return gravity * GravityMultiplierX;
	}
	float getGravityY()
	{
		return gravity * GravityMultiplierY;
	}
	void setLanded()
	{
		landed = true;
	}

	//relative movement
	void setRelativeVelocityX(float velocity)
	{
		if (WorldSprite::destinationQuadrant == 0) velocityX = velocity;
		else if (WorldSprite::destinationQuadrant == 1) velocityY = -velocity;
		else if (WorldSprite::destinationQuadrant == 2) velocityX = -velocity;
		else if (WorldSprite::destinationQuadrant == 3) velocityY = velocity;
	}
	float getRelativeVelocityX()
	{
		if (WorldSprite::destinationQuadrant == 0) return velocityX;
		else if (WorldSprite::destinationQuadrant == 1) return -velocityY;
		else if (WorldSprite::destinationQuadrant == 2) return -velocityX;
		else if (WorldSprite::destinationQuadrant == 3) return velocityY;
	}
	void setRelativeVelocityY(float velocity)
	{
		if (WorldSprite::destinationQuadrant == 0) velocityY = velocity;
		else if (WorldSprite::destinationQuadrant == 1) velocityX = velocity;
		else if (WorldSprite::destinationQuadrant == 2) velocityY = -velocity;
		else if (WorldSprite::destinationQuadrant == 3) velocityX = -velocity;
	}
	float getRelativeVelocityY()
	{
		if (WorldSprite::destinationQuadrant == 0) return velocityY;
		else if (WorldSprite::destinationQuadrant == 1) return velocityX;
		else if (WorldSprite::destinationQuadrant == 2) return -velocityY;
		else if (WorldSprite::destinationQuadrant == 3) return -velocityX;
	}
	
	//memory
	inline void setOriginalX(float x) { originalX = x; }
	inline void setOriginalY(float y) { originalY = y; }

	//reset
	virtual void reset()
	{
		velocityX = 0;
		velocityY = 0;

		image.setX(originalX);
		image.setY(originalY);
	}

protected:

	//movement
	float velocityX;
	float velocityY;
	float accelerationX;
	float gravity;
	float friction;

	//flags
	bool landed;

	//memory
	float originalX;
	float originalY;

	WorldSprite image;
	Shader* shader;
};

class Mob : public Entity
{
public:
	Mob(Shader* shader) : Entity(shader)
	{

	}
	Mob(){}

	virtual void draw(){}
	void move()
	{
		Entity::move();
	}

protected:


};

class Box : public Mob
{
public:
	Box(Shader* shader) : Mob(shader)
	{

	}
	
	void draw(){}

private:

};

class Player : public Mob
{
public:
	Player(Shader* shader) : Mob(shader)
	{
		image = WorldSprite(0, 0, 150, 150, shader, playerAnimation.getFirstAnimation());
		image.setColor(0, 0, 0);
		gravity = 0.0002;
		friction = 0.0001;
		accelerationX = 0.0004;
	}
	Player(){}

	void draw()
	{
		image.draw();
	}
	void input(Input input)
	{
		//movement input with gamepad
		destinationVelocity = (input.getGamePadAxis(GAMEPAD_LS_X) / 32768.0f) * 0.25f;

		//movement input with keyboard
		if (input.getKey(SDL_SCANCODE_A))
		{
			destinationVelocity = -0.25f;
		}
		else if (input.getKey(SDL_SCANCODE_D))
		{
			destinationVelocity = 0.25f;
		}

		//rotate screen
		if (!WorldSprite::rotateOnCollision && landed)
		{
			if (input.getGamePadButtonDown(GAMEPAD_LB) && input.getGamePadButtonDown(GAMEPAD_RB) || input.getKeyDown(SDL_SCANCODE_UP))
				WorldSprite::destinationRotation += 180.0f;
			else if (input.getGamePadButtonDown(GAMEPAD_LB) || input.getKeyDown(SDL_SCANCODE_LEFT))
				WorldSprite::destinationRotation += 90.0f;
			else if (input.getGamePadButtonDown(GAMEPAD_RB) || input.getKeyDown(SDL_SCANCODE_RIGHT))
				WorldSprite::destinationRotation -= 90.0f;
		}
	}
	void move()
	{
		//accelerate
		if (getRelativeVelocityX() > destinationVelocity)
			setRelativeVelocityX(getRelativeVelocityX() - accelerationX);
		else if (getRelativeVelocityX() < destinationVelocity)
			setRelativeVelocityX(getRelativeVelocityX() + accelerationX);
		if (abs(getRelativeVelocityX() - destinationVelocity) < accelerationX)
			setRelativeVelocityX(destinationVelocity);

		Mob::move();
	}

private:

	//movement
	float destinationVelocity;

	//animation
	Animation playerAnimation = Animation(1, 1, 1, 1, 1, 10000);
};