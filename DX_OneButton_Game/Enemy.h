#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
public:
	enum class Type
	{
		NORMAL,//倒すべき敵
		FRIEND//倒してはいけない味方
	};
private:
	float fallSpeed;
	Type m_Type;
public:
	Enemy();
	~Enemy() {};

	void Update();

	//ゲッター・セッター
	void SetType(Type type) { m_Type = type; }
	Type GetType() { return m_Type; }
};

