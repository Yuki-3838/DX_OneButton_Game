#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
public:
	enum class Type
	{
		NORMAL,//“|‚·‚×‚«“G
		FRIEND//“|‚µ‚Ä‚Í‚¢‚¯‚È‚¢–¡•û
	};
private:
	float fallSpeed;
	Type m_Type;
	int m_InvincibleTime;//–³“GŽžŠÔ
public:
	Enemy();
	~Enemy() {};

	void Update();

	//ƒQƒbƒ^[EƒZƒbƒ^[
	void SetType(Type type) { m_Type = type; }
	Type GetType() { return m_Type; }

	void SetInvincibleTime(int time) { m_InvincibleTime = time; }
	int GetInvincibleTime() { return m_InvincibleTime; }
	bool IsInvincible() { return m_InvincibleTime > 0; }
};

