#ifndef ManualQuad_HEADER
#define ManualQuad_HEADER

#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~23

class ManualQuad
{
	// Class for using a manually created quad primitive.
public:
	// Internally Accessible
	ManualQuad::ManualQuad();
	void ManualQuad::RenderAdvancedQuad();
	void ManualQuad::InitPlane();
	void ManualQuad::InitSimpleQuad();
	void ManualQuad::RenderAltQuad();
	void ManualQuad::InitShadowPlane();
	void ManualQuad::InitBlinnPlane();
	void ManualQuad::InitTransparentPlane();
	void ManualQuad::InitScreen();

	GLuint	screenQuadVAO = 0, blinnPlaneVAO = 0, 
			transparentVAO = 0;

private:
	// Externally Accessible
	GLuint	quadVAO = 0, quadVBO,
			simpleQuadVAO = 0, simpleQuadVBO,
			advancedQuadVAO = 0, aQuadVBO,
			splaneVAO = 0, splaneVBO,
			altQuadVAO = 0, altQuadVBO,
			planeVAO = 0, planeVBO,
			screenQuadVBO, blinnPlaneVBO, transparentVBO;
		
	glm::mat4 model;
	Camera cam;
};
#endif ManualQuad_HEADER
