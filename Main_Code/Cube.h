#pragma once

#include "GameObject.h"
#include "BoundingBox.h"
#include <GLFW\glfw3.h>
#include "TranslateScript.h"
#include "MeshRenderer.h"
#include "RotateScript.h"
#include "ScaleScript.h"

class Cube
{
public:
	GameObject gameObject;
	Cube(Camera* camera, const glm::mat4 &projection)
	{
		gameObject = GameObject(camera, projection);
		//Assign the model for the Cube
		Model model = Model("res/models/uvCube.obj");

		//Assign the AssetShader from the AssetManager
		AssetShader assShader = AssetManager::getShader("cube");
		//Add A Cube MeshRenderer
		gameObject.AddScript(new MeshRenderer(model, assShader));
		//Temporary Texturing is handled in MeshRenderer
		gameObject.GetScript<MeshRenderer>()->texture = AssetManager::getTexture("cubeTexture");
		
		//Add An Axis Aligned Bounding Box to the Cube
		gameObject.AddScript(new AxisAlignedBoundingBox(&gameObject));
	};

	void Update()
	{
		gameObject.Update();
	}
};