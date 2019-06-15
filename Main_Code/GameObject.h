#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "NoOatsBehaviour.h"
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include <GL\glew.h>
#include "Model.h"
#include "Camera.h"
#include "AssetManager.h"

class GameObject
{
public:
	
	//Variables
	Transform transform = Transform();
	Camera* camera;
	glm::mat4 projection;
	std::vector<GameObject *> children;
	GameObject *parent;
	std::string tag;
	vector<NoOatsBehaviour* > scripts;

	//Constructor
	GameObject() {};
	GameObject(Camera* camera, const glm::mat4 &projection);
	
	//Virtual Methods
	virtual void Update();
	virtual void DebugInfo();
	virtual void SetParent(GameObject* parent);
	virtual void DetatchFromParent();

	//Methods
	void AddScript(NoOatsBehaviour* behaviour);

	//Returns a Script of Type <T>
	template<typename CheckType>
	CheckType* GetScript()
	{
		for (int i = 0; i < scripts.size(); i++)
		{
			if (dynamic_cast<CheckType *>(scripts[i]) != NULL)
			{
				return dynamic_cast<CheckType *>(scripts[i]);
			}
		}
	}
	
	//Returns a Vector of scripts of Type <T>
	template<typename CheckType>
	vector<CheckType *> GetScripts()
	{
		vector<CheckType *> results;
		for (int i = 0; i < scripts.size(); i++)
		{
			if (dynamic_cast<CheckType *>(scripts[i]) != NULL)
			{
				results.push_back(dynamic_cast<CheckType *>(scripts[i]));
			}
		}
		return results;
	}
	
	//Removes the first occurence of Script <T> from the GameObject
	template<typename CheckType>
	void RemoveScript()
	{
		vector<NoOatsBehaviour*> cleanScripts;
		bool found = false;
		int result = -1;

		//Find the Script
		for (int i = 0; i < scripts.size(); i++)
		{
			if (!found && dynamic_cast<CheckType *>(scripts[i]) != NULL)
			{
				found = true;
				result = i;
			}
		}

		//Rebuild the Scripts Vector, excluding the found script.
		for (int i = 0; i < scripts.size(); i++)
		{
			if (i != result)
			{
				cleanScripts.push_back(scripts[i]);
			}
		}
		scripts = cleanScripts;
	}
	
	//Removes an occurance of Script <T>
	//e.g. RemoveScript<TranslateScript>(1) will remove the second TranslateScript 
	template<typename CheckType>
	void RemoveScript(int occurenceToRemove)
	{
		vector<NoOatsBehaviour*> cleanScripts;
		int result = -1;

		//Find the Instance of the Script
		for (int i = 0; i < scripts.size(); i++)
		{
			if (dynamic_cast<CheckType *>(scripts[i]) != NULL)
			{
				scripts[i]->DebugInfo();
				result++;
			}
		}

		//Rebuild the Scripts Vector without the found result.
		for (int i = 0; i < scripts.size(); i++)
		{
			if (i != result)
			{
				cleanScripts.push_back(scripts[i]);
			}
		}
		scripts[result]->DebugInfo();
		scripts = cleanScripts;
	}
	
	//Removes all occurences of Script <T> 
	template<typename CheckType>
	void RemoveAllScriptsOfType()
	{
		vector<NoOatsBehaviour*> cleanScripts;
		vector<int> results;
		for (int i = 0; i < scripts.size(); i++)
		{
			if (dynamic_cast<CheckType *>(scripts[i]) != NULL)
			{
				scripts[i]->DebugInfo();
				results.push_back(i);
			}
		}
		//For Each Script
		for (int i = 0; i < scripts.size(); i++)
		{
			bool add = true;
			//For Each Index found of that Script
			for (int j = 0; j < results.size(); j++)
			{
				//If the Script Index is any of the known Indicies containing Script <T> 
				if (i == results[j])
				{
					//Do not add it to the Clean Script vector
					add = false;
				}
			}
			//If Script Index doesn't match any results, add it to the clean scripts vector
			if (add)
			{
				cleanScripts.push_back(scripts[i]);
			}
		}
		scripts = cleanScripts;
	}
	
	//Find and return a GameObject based on it's tag
	static GameObject FindGameObjectByTag(vector<GameObject> gameObjects, string tag);
	
	//Find and return all GameObjects based on a tag
	static vector<GameObject> FindGameObjectsByTag(vector<GameObject> gameObjects, string tag);
};
#endif // !GAMEOBJECT_H