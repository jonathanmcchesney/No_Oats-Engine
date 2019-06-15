#ifndef NOOATSBEHAVIOUR_H
#define NOOATSBEHAVIOUR_H
#include <vector>
#include <iostream>

//Forward Declaration
class GameObject;

class NoOatsBehaviour
{
public:

	//Constructor
	NoOatsBehaviour() {};

	//Variables

	//The Game Object this Script is attached to
	GameObject* gameObject;

	std::vector<NoOatsBehaviour* > scripts;
	std::string name;

	//Called upon Script being Attached + Started
	virtual void Start()
	{
		for (int i = 0; i < scripts.size(); i++)
		{
			scripts[i]->Start();
		}
	};

	//Attaches this script an a GameObject
	virtual void AttachToGameObject(GameObject* gameObject)
	{
		this->gameObject = gameObject;
	}

	//Updates all subscripts (if any)
	virtual void Update()
	{
		for (int i = 0; i < scripts.size(); i++)
		{
			scripts[i]->Update();
		}
	};
	
	//Provides Debug Info
	virtual void DebugInfo()
	{
		for (int i = 0; i < scripts.size(); i++)
		{
			std::cout << "Scripts On Object Count : " << scripts.size() << std::endl;
			scripts[i]->DebugInfo();
		}
	}
};
#endif // !NOOATSBEHAVIOUR_H