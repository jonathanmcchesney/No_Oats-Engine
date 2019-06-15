#include "GameObject.h"

//Constructor
GameObject::GameObject(Camera* camera, const glm::mat4 &projection)
{
	tag = "Untagged";
	this->camera = camera;
	this->projection = projection;
	transform.Rotation = OatsPhysics::Vector3(0, 0, 0);
	transform.Translation = OatsPhysics::Vector3(0, 0, 0);
	transform.Scale = OatsPhysics::Vector3(1.0f, 1.0f, 1.0f);
}

//Update Method
void GameObject::Update()
{
	for (int i = 0; i < scripts.size(); i++)
	{
		scripts[i]->Update();
	}
}


//DebugInfo, Logs information to console
void GameObject::DebugInfo()
{
	Logging::logMessageOnly("Game Object ");
	Logging::logMessageOnly("Tag : ");
	Logging::logMessageOnly("Scripts Attached : " + scripts.size());
	for (int i = 0; i < scripts.size(); i++)
	{
		scripts[i]->DebugInfo();
	}
}

//Set Parent of this GameObject, to another GameObject
void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
	this->parent->children.push_back(this);
}

//Detach this GameObject (and it's subsequent children) from it's parent
void GameObject::DetatchFromParent()
{
	vector<GameObject *> newVector;
	for (int i = 0; i < this->parent->children.size(); i++)
	{
		if (this->parent->children[i] != this)
		{
			newVector.push_back(this->parent->children[i]);
		}
	}
	this->parent->children = newVector;
}

//Find and return GameObject based on it's tag string
GameObject GameObject::FindGameObjectByTag(vector<GameObject> gameObjects, string tag)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (tag == gameObjects[i].tag)
		{
			return gameObjects[i];
		}
	}
}

//Find and return GameObject based on it's tag string
vector<GameObject> GameObject::FindGameObjectsByTag(vector<GameObject> gameObjects, string tag)
{
	vector<GameObject> foundObjects;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (tag == gameObjects[i].tag)
		{
			foundObjects.push_back(gameObjects[i]);
		}
	}
	return foundObjects;
}

//Add a NoOatsBehaviour to this GameeObjects script vector
void GameObject::AddScript(NoOatsBehaviour* behaviour)
{
	behaviour->AttachToGameObject(this);
	scripts.push_back((behaviour));
	scripts[scripts.size() - 1]->Start();
}
