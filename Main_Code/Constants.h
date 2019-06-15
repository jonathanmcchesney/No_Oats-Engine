#ifndef Constants_HEADER
#define Constants_HEADER

#include "Variables.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~14

// This class contains constants that are used throughout the engine. If we had more time this class would not exist and more efficient ways of
// passing constants would have been implemented.
unsigned char	*imageMap, *image1;
const GLuint	WIDTH = 1280, HEIGHT = 720, SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024, POINT_SHADOW_WIDTH = 1024, POINT_SHADOW_HEIGHT = 1024;
int				SCREEN_WIDTH, SCREEN_HEIGHT, width, height;

GLboolean bloom = true, parallax_mapping = true, shadowsEnabled = true, quadDebugEnabled = true, blinn = false;

GLfloat height_scale = 0.1f, lastXPosition = WIDTH / 2.0, lastYPosition = HEIGHT / 2.0, 
		aspect = (GLfloat)POINT_SHADOW_WIDTH / (GLfloat)POINT_SHADOW_HEIGHT,
		near = 1.0f, far = 25.0f, near_plane = 1.0f, far_plane = 15.0f;

std::string diffuseMaps[] = { "pDiffuseMap4", "pDiffuseMap", "metalTexture", "pDiffuseMap6", "metalTexture", "altTexture","pDiffuseMap3" };
std::string normalMaps[] = { "pNormalMap4", "pNormalMap", "pNormalMap5", "pNormalMap6", "pNormalMap8", "pNormalMap9", "pNormalMap3" };
std::string heightMaps[] = { "pHeightMap4", "pHeightMap", "pHeightMap5", "pHeightMap6", "pNormalMap8", "pHeightMap9", "pHeightMap3" };

std::string  pointLightID;
#endif