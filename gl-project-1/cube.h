#pragma once

#include <glm.hpp>

#include <vector>

class Cube
{
public:
	glm::vec3 dimensions;
	unsigned int VBO, VAO, textures[1];
	glm::vec3 position;
	glm::mat4 model;

	bool textured;

public:
	Cube(glm::vec3 dimensions, glm::vec3 position, glm::vec3 colour);
	Cube(glm::vec3 dimensions, glm::vec3 position);
	~Cube();

	void draw();
	void setTexture(const char* filePath);
	void translate(glm::vec3 vector);
	void scale(float scalar);

private:
	void update();
};