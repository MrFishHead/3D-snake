#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <random>

#include "shader.h"
#include "camera.h"
#include "cube.h"

enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct SnakePart
{
	int x = -1, y = -1, prevX = -1, prevY = -1;
	char symbol;
	Direction direction;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// time variables
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float timers[] = { 0.0f };
unsigned int totalTimers = 1;;

// game data
SnakePart player;
Direction  lastDirection;
std::vector<SnakePart> tails;

char board[] = {
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e',
	'e','e','e','e','e','e','e','e','e','e','e'
};
const int ROWS = 11;
const int COLS = 11;

bool spawnFood = false;

// mouse variables & camera variables
Camera camera(glm::vec3((float)COLS/2.0f-0.5f, 10.0f, (float)ROWS/2.0f+3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -65.0f);
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	Shader shader("vertexShader.glsl", "fragmentShader.glsl");

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	shader.use();
	shader.setInt("texture1", 0);

	// Initialise player object
	player.x = round(COLS / 2);
	player.y = round(ROWS / 2)+1;
	player.symbol = 'h';
	player.direction = NORTH;
	bool firstMove = true;

	// Initialise snake food
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, ROWS-1);
	int randInt1 = 0;
	int randInt2 = 0;
	do
	{
		randInt1 = uni(rng);
		randInt2 = uni(rng);
	} while (board[(randInt1 * ROWS) + randInt2] != 'e');
	board[(randInt1 * ROWS) + randInt2] = 'f';

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// time variables update
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		for (unsigned int i = 0; i < totalTimers; i++)
		{
			timers[i] += deltaTime;
		}

		// input
		// -----
		processInput(window);

		// game logic
		if (timers[0] > 0.2f)
		{
			lastDirection = player.direction;

			// spawns snake food in a random location on the board and appends a snake tail
			if (spawnFood)
			{
				SnakePart tail;
				if (tails.size() > 0)
				{
					tail.x = tails.back().prevX;
					tail.y = tails.back().prevY;
				}
				else
				{
					tail.x = player.prevX;
					tail.y = player.prevY;
				}
				tail.symbol = 't';
				tails.push_back(tail);

				board[(tail.y * ROWS) + tail.x] = 't';

				do
				{
					randInt1 = uni(rng);
					randInt2 = uni(rng);
				} while (board[(randInt1 * ROWS) + randInt2] != 'e');
				board[(randInt1 * ROWS) + randInt2] = 'f';

				spawnFood = false;
			}

			player.prevX = player.x;
			player.prevY = player.y;

			// Moves the player along a direction
			switch (player.direction)
			{
			case NORTH:
				player.y -= 1;
				break;
			case EAST:
				player.x += 1;
				break;
			case SOUTH:
				player.y += 1;
				break;
			case WEST:
				player.x -= 1;
				break;
			}

			// Checks if player goes over the edge of the board and repositions them
			if (player.x > COLS - 1)
			{
				player.x = 0;
			}
			if (player.x < 0)
			{
				player.x = COLS - 1;
			}
			if (player.y > ROWS - 1)
			{
				player.y = 0;
			}
			if (player.y < 0)
			{
				player.y = ROWS - 1;
			}

			// Moves each tail tile
			if (tails.size() > 0)
			{
				tails.front().prevX = tails.front().x;
				tails.front().x = player.prevX;
				tails.front().prevY = tails.front().y;
				tails.front().y = player.prevY;
			}
			for (unsigned int i = 1; i < tails.size(); i++)
			{
				tails.at(i).prevX = tails.at(i).x;
				tails.at(i).x = tails.at(i - 1).prevX;
				tails.at(i).prevY = tails.at(i).y;
				tails.at(i).y = tails.at(i - 1).prevY;
			}

			// Checks if snake head collided with its tail
			if (board[(player.y * ROWS) + player.x] == 't')
			{
				glfwSetWindowShouldClose(window, true);
			}

			// Checks if snake ate food
			if (board[(player.y * ROWS) + player.x] == 'f')
			{
				spawnFood = true;
			}

			// Mark snake entities on the board to be drawn
			board[(player.y * ROWS) + player.x] = player.symbol;
			for (unsigned int i = 0; i < tails.size(); i++)
			{
				board[(tails.at(i).y * ROWS) + tails.at(i).x] = 't';
			}

			// Clear previously occupied board spaces
			if (tails.size() <= 0)
			{
				board[(player.prevY * ROWS) + player.prevX] = 'e';
			}
			else if (tails.size() > 0 && tails.back().prevX != -1)
			{
				board[(tails.back().prevY * ROWS) + tails.back().prevX] = 'e';
			}

			timers[0] = 0.0f;
		}

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		shader.use();

		// Create Transformations
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// set uniforms
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (unsigned int i = 0; i < ROWS; i++)
		{
			for (unsigned int j = 0; j < COLS; j++)
			{
				char index = board[(i * ROWS) + j];
				Cube* cube;
				switch (index)
				{
				case 'e':
					cube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3((float)j, 0.0f, (float)i), glm::vec3(0.1f, 0.1f, 0.1f));
					break;
				case 'f':
					cube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3((float)j, 1.0f, (float)i), glm::vec3(1.0f, 0.0f, 0.0f));
					break;
				case 't':
				case 'h':
					cube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3((float)j, 1.0f, (float)i), glm::vec3(0.0f, 1.0f, 0.0f));
					break;
				default:
					cube = new Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3((float)j, 0.0f, (float)i), glm::vec3(1.0f, 0.5f, 0.5f));
				}

				shader.setBool("textured", cube->textured);
				shader.setMat4("model", (*cube).model);
				cube->draw();
				delete cube;
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (lastDirection == EAST || lastDirection == WEST))
		player.direction = NORTH;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (lastDirection == NORTH || lastDirection == SOUTH))
		player.direction = EAST;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (lastDirection == EAST || lastDirection == WEST))
		player.direction = SOUTH;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (lastDirection == NORTH || lastDirection == SOUTH))
		player.direction = WEST;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}