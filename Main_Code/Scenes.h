#include "Model.h"
#include "AssetManager.h"
#include "Constants.h"
#include "Camera.h"
#include "ParticleTrail.h"

// Created by Jonathan McChesney
// Jonathan McChesney LOC : ~28

class Scenes
{
public:
	// Accessible Externally
	Scenes::Scenes();
	void Scenes::RenderScene01(AssetShader modelShader, Camera cam, bool activeRoom);
	void Scenes::RenderDoorScene(AssetShader modelShader, AssetShader stencilShader, Camera cam, bool isStencil, glm::mat4 &projection);
	void Scenes::DrawStandScene(AssetShader modelShader, Camera cam);
	void Scenes::DrawReflectionModelScene(AssetShader modelShader, Camera cam, glm::mat4 &projection);
	
	Model	reflectiveModel, shadowModel, fountainModel, standModel, stand2Model, chair1Model, chair2Model, table1Model,
			muffinModel, barrierModel, chainModel, statue1Model, statue2Model, plateModel, simpleSphereModel, doorModel, cigModel;

private:
	// Accessible Internally
	void Scenes::UpdateReflectionModelScene();
	void Scenes::DrawDinoScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateDinoScene();
	void Scenes::DrawFountainScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateFountainScene();
	void Scenes::UpdateStandScene(int totalItems);
	void Scenes::DrawBarrierScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateBarrierScene(int totalItems);
	void Scenes::DrawChairScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateChairScene(int totalItems);
	void Scenes::DrawChainScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateChainScene(int totalItems);
	void Scenes::DrawStatueScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateStatueScene(int totalItems);
	void Scenes::UpdateDoorScene(int totalItems);
	void Scenes::DrawCigScene(AssetShader modelShader, Camera cam);
	void Scenes::UpdateCigScene();
	
	GLfloat distancePoint;
	glm::mat4 model;
	Camera cam;
};