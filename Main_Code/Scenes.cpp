#include "Scenes.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~390

// Scenes constructor
Scenes::Scenes()
{
	glm::mat4 projection = projection;
}

// This method allows us to render a scene. Ideally we would liked to have used a constructor to set up the scene, and when we interacted with a particular object within the scene,
// i.e. the door a deconstructor would be used for clean up and we would then render out our second scene. This would have been implemented if we had more time.
// We enable face culling here and then draw all the elements that we wish to be part of this particular scene. In this case its the top floor of the museum.
void Scenes::RenderScene01(AssetShader modelShader, Camera cam, bool activeRoom) 
{
	glEnable(GL_CULL_FACE);
	DrawDinoScene(modelShader, cam);
	DrawBarrierScene(modelShader, cam);
	DrawChairScene(modelShader, cam);
	DrawFountainScene(modelShader, cam);
	DrawStatueScene(modelShader, cam);
	DrawCigScene(modelShader, cam);
	if(activeRoom)DrawChainScene(modelShader, cam);
}

// This allows us to update the location and scale of our model(s).
void Scenes::UpdateReflectionModelScene() 
{
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::translate(glm::rotate(model, glm::radians(gDegreesRotated), glm::vec3(0, 1, 0)), glm::vec3(3.0f, 3.0f, 0.0f)); // translate then rotate using glm:- fixes the need to translate and then retranslate after
}

// This scene draws our sphere model with an applied environment map. We first instantiate our distance point (the length between the cameras position and the center of the scenes location).
// We use our model shader and assign our uniforms as normal, update the models positioning, along with activating our texture (3), assigning the skybox and model uniforms as appropraite when binding our texture.
// Depending on our distance from the scene we either use a detailed sphere model, or a less detailed model (further away).
void Scenes::DrawReflectionModelScene(AssetShader modelShader, Camera cam, glm::mat4 &projection)
{
	distancePoint = glm::length(cam.GetPosition() - (glm::vec3)modelLocation);
	modelShader.use();

	modelShader.setMat4("view", cam.GetViewMatrix());
	modelShader.setMat4("projection", projection);
	modelShader.setVec3f("cameraPos", cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
	UpdateReflectionModelScene();

	glActiveTexture(GL_TEXTURE3);
	modelShader.setInt("skybox", 3);
	modelShader.setMat4("model", model);

	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("metalTexture"));

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDisable(GL_CULL_FACE);
	if (distancePoint > 35) 
		simpleSphereModel.Draw(modelShader);
	else 
		reflectiveModel.Draw(modelShader); 
}

// This allows us to update the location and scale of our model(s).
void Scenes::UpdateDinoScene() 
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(4.0f, -0.49f, 4.0f));
	model = glm::scale(model, glm::vec3(0.02f));
}

// This scene renders our dinosuar model to screen.
void Scenes::DrawDinoScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateDinoScene();

	modelShader.setMat4("model", model);
	shadowModel.Draw(modelShader);
}

// This allows us to update the location and scale of our model(s).
void Scenes::UpdateFountainScene() 
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-4.0f, -0.49f, 4.0f));
	model = glm::scale(model, glm::vec3(0.02f));
}

// This renders the fountain model to screen.
void Scenes::DrawFountainScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateFountainScene();

	modelShader.setMat4("model", model);
	fountainModel.Draw(modelShader);
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateDoorScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(-6.95f, -0.49f, -2.0f));
		model = glm::scale(model, glm::vec3(0.0009f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(-6.95f, -0.49f, -2.0f));
		model = glm::scale(model, glm::vec3(0.0009f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This renders our door to the scene. We use the stencil function and mask to render (using 1s and 0s to decide where we render to), this allows us to render a single colour on top of the
// the door model to create a highlighting effect when the stencil boolean is active.
void Scenes::RenderDoorScene(AssetShader modelShader, AssetShader stencilShader, Camera cam, bool isStencil, glm::mat4 &projection)
{
	stencilShader.use();
	stencilShader.setMat4("view",view);
	stencilShader.setMat4("projection", projection);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateDoorScene(1);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("rust1Texture"));
	doorModel.Draw(modelShader);
	
	if (isStencil)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		stencilShader.use();

		UpdateDoorScene(1);
		stencilShader.setMat4("model", model);
		doorModel.Draw(modelShader);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateStandScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(-5.0f, -0.49f, 1.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(3.7f, -0.49f, 3.5f));
		model = glm::scale(model, glm::vec3(0.007f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This renders our stands (beside the dinosaur and holding the seperate parallax image).
void Scenes::DrawStandScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateStandScene(0);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("cubeTexture"));
	standModel.Draw(modelShader);

	UpdateStandScene(1);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("metalTexture"));
	stand2Model.Draw(modelShader);
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateChainScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(-4.15f, 0.66f, 0.5f));
		model = glm::scale(model, glm::vec3(0.064f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(9.85f, 0.66f, 6.0f));
		model = glm::scale(model, glm::vec3(0.064f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 2) 
	{
		model = glm::translate(model, glm::vec3(9.80f, 0.66f, 0.5f));
		model = glm::scale(model, glm::vec3(0.064f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 3) 
	{
		model = glm::translate(model, glm::vec3(-4.15f, 0.66f, 6.0f));
		model = glm::scale(model, glm::vec3(0.064f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 4) 
	{
		model = glm::translate(model, glm::vec3(3.1f, 3.3f, 3.3f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 5) 
	{
		model = glm::translate(model, glm::vec3(0.0f, 3.1f, -3.3f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 6) 
	{
		model = glm::translate(model, glm::vec3(1.1f, 3.01f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 7) 
	{
		model = glm::translate(model, glm::vec3(1.1f, 1.67f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 8) 
	{
		model = glm::translate(model, glm::vec3(1.1f, 1.67f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 9) 
	{
		model = glm::translate(model, glm::vec3(5.0f, 3.67f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This renders the chains holding up exhibits and the walls around the museum.
void Scenes::DrawChainScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("rust1Texture"));

	for (int i = 0; i < 9; i++) 
	{
		UpdateChainScene(i);
		modelShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		if (i == 2) 
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("rust2Texture"));
		}
		chainModel.Draw(modelShader);
	}
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateBarrierScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(3.0f, -0.49f, 2.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(3.0f, -0.49f, 1.1f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 2) 
	{
		model = glm::translate(model, glm::vec3(3.0f, -0.49f, 0.2f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 3) 
	{
		model = glm::translate(model, glm::vec3(3.0f, -0.49f, -0.7f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 4) 
	{
		model = glm::translate(model, glm::vec3(3.0f, -0.49f, -1.6f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 5) 
	{
		model = glm::translate(model, glm::vec3(-2.1f, -0.49f, 2.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 6) 
	{
		model = glm::translate(model, glm::vec3(-2.1f, -0.49f, 1.1f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 7) 
	{
		model = glm::translate(model, glm::vec3(-2.1f, -0.49f, -0.7f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 8) 
	{
		model = glm::translate(model, glm::vec3(-2.1f, -0.49f, -1.6f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 9) 
	{
		model = glm::translate(model, glm::vec3(-2.1f, -0.49f, 0.2f));
		model = glm::scale(model, glm::vec3(0.08f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This renders the barriers seperating sections of the museum.
void Scenes::DrawBarrierScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("artTexture"));
	
	for (int i = 0; i < 10; i++) 
	{
		UpdateBarrierScene(i);
		modelShader.setMat4("model", model);

		barrierModel.Draw(modelShader);
	}
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateStatueScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(-4.8f, -0.49f, 3.0f));
		model = glm::scale(model, glm::vec3(0.015f));
		model = glm::rotate(model, 110.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(-5.0f, -0.49f, 5.4f));
		model = glm::scale(model, glm::vec3(0.015f));
		model = glm::rotate(model, 89.5f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 2) 
	{
		model = glm::translate(model, glm::vec3(6.85f, 0.30f, 3.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 3) 
	{
		model = glm::translate(model, glm::vec3(6.85f, 0.30f, 1.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 4) 
	{
		model = glm::translate(model, glm::vec3(6.85f, 0.30f, 5.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 5) 
	{
		model = glm::translate(model, glm::vec3(6.85f, 0.30f, -0.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 6) 
	{
		model = glm::translate(model, glm::vec3(6.85f, 0.30f, -2.5f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This renders the various statues and wall mounts around the museum (different materials - textures).
void Scenes::DrawStatueScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateStatueScene(0);
	modelShader.setMat4("model", model); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
	statue1Model.Draw(modelShader);

	UpdateStatueScene(1);
	modelShader.setMat4("model", model); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("jadeTexture"));
	statue1Model.Draw(modelShader);

	UpdateStatueScene(2);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("graniteTexture"));
	statue2Model.Draw(modelShader);

	UpdateStatueScene(3);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
	statue2Model.Draw(modelShader);

	UpdateStatueScene(4);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("velvetTexture"));
	statue2Model.Draw(modelShader);

	UpdateStatueScene(5);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("goldTexture"));
	statue2Model.Draw(modelShader);

	UpdateStatueScene(6);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("rust1Texture"));
	statue2Model.Draw(modelShader);
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateChairScene(int totalItems) 
{
	model = glm::mat4();
	if (totalItems == 0) 
	{
		model = glm::translate(model, glm::vec3(6.5f, 0.145f, 1.5f));
		model = glm::scale(model, glm::vec3(0.01));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 1) 
	{
		model = glm::translate(model, glm::vec3(6.5f, -0.49f, 1.48f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 2) 
	{
		model = glm::translate(model, glm::vec3(6.37f, 0.184f, 1.36f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 3) 
	{
		model = glm::translate(model, glm::vec3(6.49, 0.180f, 1.71f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 4) 
	{
		model = glm::translate(model, glm::vec3(6.40, 0.180f, 1.72f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 5) 
	{
		model = glm::translate(model, glm::vec3(6.505f, 0.180f, 1.62f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 6) 
	{
		model = glm::translate(model, glm::vec3(6.42f, 0.180f, 1.6f));
		model = glm::scale(model, glm::vec3(0.0008f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 7) 
	{
		model = glm::translate(model, glm::vec3(6.5f, -0.49f, 2.6f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
	else if (totalItems == 8) 
	{
		model = glm::translate(model, glm::vec3(6.5f, -0.49f, 0.6f));
		model = glm::scale(model, glm::vec3(0.01f));
		model = glm::rotate(model, 55.3f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
	}
}

// This draws the chairs with a fabric texture applied.
void Scenes::DrawChairScene(AssetShader modelShader, Camera cam) 
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateChairScene(0);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
	plateModel.Draw(modelShader);

	UpdateChairScene(1);
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
	table1Model.Draw(modelShader);

	for (int i = 2; i < 7; i++) 
	{
		UpdateChairScene(i);
		modelShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("altTexture"));
		muffinModel.Draw(modelShader);
	}
	
	for (int j = 7; j < 9; j++) 
	{
		UpdateChairScene(j);
		modelShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("fabricTexture"));
		chair1Model.Draw(modelShader);
	}
}

// This allows us to update the location, rotation and scale of our model(s).
void Scenes::UpdateCigScene()
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.5f, 0.97f, 4.45f));
	model = glm::scale(model, glm::vec3(2.0));
	model = glm::rotate(model, 0.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));

}

// This draws the cigarette model with a white fabric texture applied.
void Scenes::DrawCigScene(AssetShader modelShader, Camera cam)
{
	modelShader.use();
	modelShader.setMat4("view", cam.GetViewMatrix());

	UpdateCigScene();
	modelShader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AssetManager::getTexture("whiteFabric"));
	glDisable(GL_CULL_FACE);
	cigModel.Draw(modelShader);
	glEnable(GL_CULL_FACE);
}