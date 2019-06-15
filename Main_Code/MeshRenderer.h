#pragma once
#include "NoOatsBehaviour.h"
#include "Model.h"
#include "Shader.h"
#include "GameObject.h"
#include "gtc\type_ptr.hpp"
#include "AssetShader.h"
#include "AssetManager.h"
#include "Constants.h"
#include "gtx\quaternion.hpp"

class MeshRenderer : public NoOatsBehaviour
{
public:
	//The Model to Render
	Model model;

	//The Shader to Apply to the Model
	AssetShader shader;
	
	//The Texture to Apply to the Model
	GLuint texture;
	
	//Default Constructor
	MeshRenderer();

	//Configurable Constructor
	MeshRenderer(Model model, AssetShader &shader);

	void Update();
};