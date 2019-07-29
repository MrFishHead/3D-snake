#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "cube.h"
#include <gtc/matrix_transform.hpp>

#include <iostream>

Cube::Cube(glm::vec3 dimensions, glm::vec3 position, glm::vec3 colour)
{
	float vertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		// forward face					  
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		// left face					  
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		// right face					  
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 // bottom face					  
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		// top face						  
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  colour.x, colour.y, colour.z,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  colour.x, colour.y, colour.z
	};

	this->dimensions = dimensions;
	this->position = position;
	textured = false;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, dimensions);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::Cube(glm::vec3 dimensions, glm::vec3 position)
{
	float vertices[] = {
	// back face
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	// forward face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// left face
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// right face
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 // bottom face
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// top face
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};

	this->dimensions = dimensions;
	this->position = position;
	textured = false;

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, dimensions);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Cube::draw()
{
	if (textured)
	{
		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	}
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::setTexture(const char* filePath)
{
	// Texture handling
	glGenTextures(1, textures);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	textured = true;
}

void Cube::translate(glm::vec3 vector)
{
	position += vector;
	update();
}

void Cube::scale(float scalar)
{
	dimensions *= scalar;
	update();
}

void Cube::update()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, dimensions);
}