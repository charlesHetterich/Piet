#pragma once
#include "globalVariablesAndIncludes.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Input.h"

class StencilObject
{
public:
	StencilObject(GLenum stencilFuncType, Shader* shader, std::vector<Vertex> vertices) : 
			shader(shader), stencilFuncType(stencilFuncType)
	{
		xPos = 0;
		yPos = 0;
		rotation = 0;

		std::vector<GLuint> indicies;
		
		for (unsigned int i = 0; i < vertices.size(); i++)
			indicies.push_back(i);

		mesh.reset(new Mesh(vertices, vertices.size(), indicies, indicies.size()));
	}
	StencilObject(){}

	//binding
	void bind()
	{
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		drawStencil();

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(stencilFuncType, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}
	void unbind()
	{
		glDisable(GL_STENCIL_TEST);
	}

	//transform
	inline void setX(float x) { xPos = x; }
	inline float getX() { return xPos; }
	inline void setY(float y) { yPos = y; }
	inline float getY() { return yPos; }

protected:

	void drawStencil()
	{
		shader->bind();
		shader->update(getTransform());
		mesh->draw();
	}
	virtual glm::mat4 getTransform()
	{
		return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::translate(glm::vec3(xPos, yPos, 0)) * glm::rotate(rotation, glm::vec3(0, 1, 0));
	}

	//position
	float xPos;
	float yPos;

	//rotation
	float rotation;

	Shader* shader;
	GLenum stencilFuncType;
	std::shared_ptr<Mesh> mesh;
};

class WorldStencilObject : public StencilObject
{
public:
	//constant worldly things
	static float* cameraFocusX;
	static float* cameraFocusY;
	static float* cameraRotation;

	WorldStencilObject(GLenum stencilFuncType, Shader* shader, std::vector<Vertex> verticies) : StencilObject(stencilFuncType, shader, verticies){}
	WorldStencilObject(){}
private:
	glm::mat4 getTransform()
	{
		return glm::scale(glm::vec3(1 / SCREEN_WIDTH, 1 / SCREEN_HEIGHT, 1)) * glm::rotate(*cameraRotation, glm::vec3(0, 0, 1)) * glm::translate(glm::vec3(xPos - (int)*cameraFocusX, yPos - (int)*cameraFocusY, 0)) * glm::rotate(rotation, glm::vec3(0, 0, 1));
	}
};

