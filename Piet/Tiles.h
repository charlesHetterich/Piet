#pragma once
#include "globalVariablesAndIncludes.h"
#include "Entities.h"

class Block
{
public:
	Block(){}

	virtual void draw()
	{
		block.draw();
	}
	
	virtual float ObjectCollision(Entity* entity)
	{
		Rect rect = entity->getImage()->getRect();
		if (AABB(block.getRect(), entity->getImage()->getRect()))
		{
			entity->setLanded();

			//if collision is due to negative x velocity
			if (rect.x - entity->getVelocityX() >= block.getX() + block.getW())
			{
				entity->getImage()->setX(block.getX() + block.getW());
				entity->setVelocityX(0);
				return 90;
			}

			//if collision is due to positive x velocity
			else if (rect.x + rect.w - entity->getVelocityX() <= block.getX())
			{
				entity->getImage()->setX(block.getX() - rect.w);
				entity->setVelocityX(0);
				return 270;
			}

			//if collision is due to negative y velocity
			else if (rect.y - entity->getVelocityY() >= block.getY() + block.getH())
			{
				entity->getImage()->setY(block.getY() + block.getH());
				entity->setVelocityY(0);
				return 0;
			}

			//if collision is due to positive y velocity
			else if (rect.y + rect.h - entity->getVelocityY() <= block.getY())
			{
				entity->getImage()->setY(block.getY() - rect.h);
				entity->setVelocityY(0);
				return 180;
			}

			return WorldSprite::destinationRotation;
		}
	}
	virtual void playerCollision(Player* player)
	{
		//get collision
		if (WorldSprite::rotateOnCollision)
			WorldSprite::destinationRotation = ObjectCollision(player);
		else
			ObjectCollision(player);

		//rotate screen correctly
		if (WorldSprite::destinationRotation - WorldSprite::cameraRotation > 180)
			WorldSprite::cameraRotation += 360;
		else if (WorldSprite::destinationRotation - WorldSprite::cameraRotation < -180)
			WorldSprite::cameraRotation -= 360;
	}

protected:
	
	WorldSprite block;
	Animation animation;
};

class RedBlock : public Block
{
public:
	RedBlock(float x, float y, float w, float h, Shader* shader) : Block()
	{
		animation = Animation(1, 1, 1, 1, 1, 1000);
		block = WorldSprite(x, y, w, h, shader, animation.getFirstAnimation());
		block.setColor(1.0f, 0.0f, 0.0f);
	}

private:

};

class YellowBlock : public Block
{
public:
	YellowBlock(float x, float y, float w, float h, Shader* shader) : Block()
	{
		animation = Animation(1, 1, 1, 1, 1, 1000);
		block = WorldSprite(x, y, w, h, shader, animation.getFirstAnimation());
		block.setColor(0.882f, 0.901f, 0.404f);
	}
private:

};

class BlueBlock : public Block
{
public:
	BlueBlock(float x, float y, float w, float h, Shader* shader) : Block()
	{
		animation = Animation(0, 0, Animation::CANVAS_WIDTH, Animation::CANVAS_HEIGHT, 1, 1000);
		block = WorldSprite(x, y, w, h, shader, animation.getFirstAnimation());
		block.setColor(0.0f, 0.0f, 1.0f);
		block.setAlpha(0.5f);
	}

	void draw()
	{
		block.drawWave();
	}
	
	float ObjectCollision(Entity* entity)
	{
		Rect rect = entity->getImage()->getRect();
		if (AABB(block.getRect(), entity->getImage()->getRect()))
		{
			entity->setLanded();

			entity->setRelativeVelocityY(entity->getRelativeVelocityY() + 2 * entity->getGravity());

			//if no collision is due to negative x velocity
			if (rect.x + entity->getVelocityX() >= block.getX() + block.getW() || rect.x - entity->getVelocityX() >= block.getX() + block.getW())
			{
				return 90;
			}

			//if no collision is due to positive x velocity
			else if (rect.x + rect.w + entity->getVelocityX() <= block.getX() || rect.x + rect.w - entity->getVelocityX() <= block.getX())
			{
				return 270;
			}

			//if no collision is due to negative y velocity
			if (rect.y + entity->getVelocityY() >= block.getY() + block.getH() || rect.y - entity->getVelocityY() >= block.getY() + block.getH())
			{
				return 0;
			}

			//if no collision is due to positive y velocity
			else if (rect.y + rect.h + entity->getVelocityY() <= block.getY() || rect.y + rect.h - entity->getVelocityY() <= block.getY())
			{
				return 180;
			}
		}
		return WorldSprite::destinationRotation;
	}

private:

};