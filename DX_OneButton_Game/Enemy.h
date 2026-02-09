#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
private:
	float fallSpeed;
public:
	Enemy();
	~Enemy() {};

	void Update();
};

