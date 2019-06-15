#ifndef BUFFERS_HEADER
#define BUFFERS_HEADER
#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ManualQuad.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~15

class Buffers
{
	// Header class for the buffers (Interface), public and private access as appropriate.
public:
	// Accessible externalls
	Buffers::Buffers();
	void Buffers::SetupShadowBuffers();
	void Buffers::SetupFrameBuffers();
	void Buffers::RenderFrameBufferScene(AssetShader fbShader, AssetShader fbCameraShader, AssetShader fbCameraShader2, GLuint screenTexture, int currentCamera);
	ManualQuad Buffers::quad;

private:
	// Accessible internally
	GLuint Buffers::generateMultiSampleTexture(GLuint numOfSamples);
	GLuint Buffers::generateAttachmentTexture(GLboolean depthEnabled, GLboolean stencilEnabled);

	Camera cam;
};
#endif