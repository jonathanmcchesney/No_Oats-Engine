/*  -- Uncomment this, and comment main.cpp to enable this.

#include <iostream>
#include <string>
#include <cmath>

#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm//gtc/type_ptr.hpp"

#include <GL/glew.h>
#include <random>

#include "SOIL2/SOIL2.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

#include "Transform.h"
#include "Cube.h"
#include "CollisionDetection.h"

//Resources used
// https://open.gl/
// https://learnopengl.com/
// Sonar Systems YouTube Tutorial's on Modern OpenGL.

	//Window Width / Height
	const GLint WIDTH = 800, HEIGHT = 600;
	int SCREEN_WIDTH, SCREEN_HEIGHT;


	void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	void MouseCallback(GLFWwindow *window, double xPos, double yPos);
	void DoMovement();

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	GLfloat lastX = WIDTH / 2.0f;
	GLfloat lastY = WIDTH / 2.0f;

	bool keys[1024];
	bool firstMouse = true;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	//GLfloat cubeScaleFactorX = 1.0f, cubeScaleFactorY = 1.0f, cubeScaleFactorZ = 1.0f;
	//GLfloat cubeTranslateX = 0.0f, cubeTranslateY = -1.75f, cubeTranslateZ = 0.0f;

	std::vector<Cube> cubes;


int main()
	{
#pragma region Setup
		//Initializes GLFW
		if (!glfwInit())
		{
			std::cout << "Failed to Initialize GLFW" << std::endl;
			return EXIT_FAILURE;
		};

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Tries to set the version of GLFW to 3.3?
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Sets the OpenGL Profile to Core, not Compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Forward Compatability (For Mac compatibility)
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Window not resizable

		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ELE 4014 OpenGL", NULL, NULL); //Creates an OpenGL Window

																							 //Set the Key callback for Escape key to quit window
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, MouseCallback);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT); //Gets the size of the screen relative to pixel density changes

		if (!window)
		{
			std::cout << "GLFW Window creation failed" << std::endl;
			glfwTerminate();

			return EXIT_FAILURE;
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;

		if (GLEW_OK != glewInit())//Initialize GLEW
		{
			std::cout << "Intialization of GLEW Failed" << std::endl;

			return EXIT_FAILURE;
		}
#pragma endregion

		//Setting up Viewport
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		//Enabled for Skybox
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Shaders
		Shader skyboxShader("res/shaders/skybox.vertex", "res/shaders/skybox.frag");
		Shader sphereShader("res/shaders/sphere.vertex", "res/shaders/sphere.frag");
		Shader shader("res/shaders/modelLoading.vertex", "res/shaders/modelLoading.frag");
		Shader cubeShader("res/shaders/lighting.vertex", "res/shaders/lighting.frag");
		Shader lightShader("res/shaders/lamp.vertex", "res/shaders/lamp.frag");

		//Models
		Model sphere("res/models/sphere.obj");


		//Textures
		GLuint cubeTexture = TextureLoading::LoadTexture("res/images/container2.png");
		GLuint cubeSpecular = TextureLoading::LoadTexture("res/images/container2_specular.png");

#pragma region SkyboxVerts
		GLfloat skyboxVertices[] = {
			// Positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};
#pragma endregion

#pragma region Skybox Setup
		//SkyBox
		GLuint skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glBindVertexArray(0);

		vector<const GLchar *> faces;
		faces.push_back("res/images/Skybox/ae_rt.tga");
		faces.push_back("res/images/Skybox/ae_lf.tga");
		faces.push_back("res/images/Skybox/ae_up.tga");
		faces.push_back("res/images/Skybox/ae_dn.tga");
		faces.push_back("res/images/Skybox/ae_bk.tga");
		faces.push_back("res/images/Skybox/ae_ft.tga");


		GLuint skyboxTexture = TextureLoading::LoadCubemap(faces);
#pragma endregion

#pragma region CubeVerts
		GLfloat cubeVertices[] =
		{
			//Position Co-Ords		//Normals				//Texture Co-Ords
			-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
			0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

			0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
			0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
			0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
			0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
			0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
			0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
		};
#pragma endregion

#pragma region Cube Setup

		//Cube
		GLuint cubeVAO, cubeVBO;
	/*	glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		cubeShader.UseCurrentShader();
		glUniform1i(glGetUniformLocation(cubeShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(cubeShader.Program, "material.specular"), 1);
		
#pragma endregion

#pragma region Light Setup
		GLuint lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glGenBuffers(1, &lightVAO);
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBindVertexArray(lightVAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

#pragma endregion

		//Projection
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


#pragma region GameLoop

		//"Game" Loop. 
		while (!glfwWindowShouldClose(window))
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			DoMovement();

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Skybox
			//Skybox -- Setup
			glDepthFunc(GL_LEQUAL);
			skyboxShader.UseCurrentShader();


			//Skybox -- Model View Projection Matrix
			glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


			//Skybox -- Binding Textures
			glBindVertexArray(skyboxVAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);

#pragma endregion

			for (int i = 0; i < cubes.size(); i++)
			{
			
				if (cubes.size() >= 1)
				{
					cubes[0].enabled = false;
				}

			cubes[i].Update(keys, camera, projection);

			if (cubes.size() >= 2)
			{
				AABBCollisionDetection::TestAABBToAABB(cubes[0].AABB, cubes[1].AABB);
			}
			 cubes[i].Draw();
			}

			//Unbinding Vertex Array
			glBindVertexArray(0);

			//Swap the Frame Buffers
			glfwSwapBuffers(window);
		}
#pragma endregion

		std::cout << "Exit Loop" << std::endl;


		//Cleanup -- Deleting Arrays
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteVertexArrays(1, &lightVAO);
		glDeleteVertexArrays(1, &cubeVAO);

		//Cleanup -- Deleting Buffers
		glDeleteBuffers(1, &cubeVBO);
		glDeleteBuffers(1, &skyboxVBO);


		glfwTerminate();

		return EXIT_SUCCESS;
	}

	void DoMovement()
	{
		//Camera Movement
		if (keys[GLFW_KEY_W])
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}

		if (keys[GLFW_KEY_S])
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}

		if (keys[GLFW_KEY_A])
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}

		if (keys[GLFW_KEY_D])
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}

		if (keys[GLFW_KEY_1])
		{
			cubes.push_back(Cube::CreateCube(Cube::TextureType::TEXTURE, camera.GetPosition()));
		}

		if (keys[GLFW_KEY_2])
		{
			cubes.push_back(Cube::CreateCube(Cube::TextureType::SOLID_COLOUR, camera.GetPosition()));
		}

	
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
			{
				keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				keys[key] = false;
			}

		}
	}

	void MouseCallback(GLFWwindow *window, double xPos, double yPos)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		GLfloat xOffset = xPos - lastX;
		GLfloat yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;

		camera.ProcessMouseMovement(xOffset, yOffset);
	}

	*/