#include "Sprite.h"

Sprite::Sprite(float x, float y, float w, float h, Shader* shader, Rect firstAnim)
{
	rect = { x, y, w, h };
	Vertex positions[] = {
			{ -0.5f, -0.5f, 0.0f, 1, 1, 1, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1, 1, 1, 1.0f, 1.0f },
			{ 0.5f, 0.5f, 0.0f, 1, 1, 1, 1.0f, 0.0f },
			{ -0.5f, 0.5f, 0.0f, 1, 1, 1, 0.0f, 0.0f },
	};

	 GLuint indicies[] = { 0, 1, 2, 3 };

	 mesh.reset(new Mesh(positions, sizeof(positions) / sizeof(positions[0]), indicies, sizeof(indicies) / sizeof(indicies[0])));

	 mesh->setTexCoords(firstAnim);
	 this->shader = shader;
	 rotation = 0;
}

Sprite::~Sprite()
{
}

void Sprite::draw(Animation* animation)
{
	shader->bind();
	shader->update(getTransform(), getColor());
	if (animation->shouldAnimate())
		mesh->setTexCoords(animation->getAnimation());
	mesh->draw();
}

void Sprite::draw()
{
	shader->bind();
	shader->update(getTransform(), getColor());
	mesh->draw();
}

void Sprite::drawWave()
{
	shader->bind();
	shader->update(getTransform(), glm::vec2(rect.w / 200, rect.h / 200), getColor());
	mesh->draw();
}

glm::mat4 Sprite::getTransform()
{
	return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::translate(glm::vec3(rect.x, rect.y, 0)) *glm::scale(glm::vec3(rect.w, rect.h, 1)) * glm::rotate(rotation, glm::vec3(0, 1, 0));
}