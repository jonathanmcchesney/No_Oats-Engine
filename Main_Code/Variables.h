#ifndef GlobalVariables_HEADER
#define GlobalVariables_HEADER
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <random>

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~26

// This class contains variables that are used throughout the engine. If we had more time this class would not exist and more efficient ways of
// passing variables would have been implemented.
const GLfloat	degreesPerSecond = 180.0f, exposure = 1.0f;
static int		leftClick = 0, rightClick = 0, currentCamera = 0, imageInt = 0;
int				nbFrames = 0, mode = 0;
double			lastTime = glfwGetTime();
bool			keys[1024], keysPressed[1024], isStencil = false, noclip = true, slowWalk = false,
				firstMouseMovement = true, activeRoom = true, normalMapping = true, 
				toggleShadowBlinn = true, isWindowsSetup = false, isPerspective = true;

GLuint	viewPositionLocation, modelLocation, viewLocation, projectionLocation,
		screenQuadVAO = 0, screenQuadVBO;

GLuint	depthMapFBO, depthMap, pointDepthMapFBO, depthCubemap, framebuffer, buffer, renderBufferObject,
		sampledTextureAttachment, secondaryFramebuffer, screenTexture, instancingVertexArrayObject,
		cubeTexture, floorTexture, transparentTexture, nDiffuseMap, nNormalMap, 
		pDiffuseMap, pNormalMap, pHeightMap, diffuseMap, specularMap, emissionMap, cubemapTexture;

GLfloat gDegreesRotated = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f, currentFrame,
		angle, angleOverTime, rotateAmount;

glm::mat4 lightProjection, lightView, lightSpaceMatrix, shadowProjections, view,  model;
glm::vec3	lightPosition(0.0f, 6.0f, 0.0f), lightPos(0.0f, 0.0f, 0.0f), nLightPos(0.0f, 1.0f, -2.7f),
			pLightPos(-5.5f, 1.5f, 0.2f), slightPos(-2.0f, 4.0f, -1.0f), pslightPos(0.0f, 0.0f, 0.0f),
			pointLightPositions[] = { glm::vec3(-2.0f,  2.0f,  2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(-2.0f,  2.0f, -2.0f), glm::vec3(2.0f,  2.0f, -2.0f) },
			cubePositions[] = { glm::vec3(2.0f, 1.0f, -1.0f) };

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<int> uni(0, 5);
std::vector<glm::mat4> shadowTransformations;
std::string fps = "";

auto random_integer = uni(rng), random_integer_2 = uni(rng);
#endif