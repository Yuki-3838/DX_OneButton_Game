#pragma once
#include "Character.h"

class Missile :public Character
{
public:
	enum class Type
	{
		MAIN,//縦に飛ぶ
		SUB, //横に飛ぶ
	};
private:
	Type m_Type;;

public:

	Missile(Type type);
	~Missile() {};

	void Update();

	//ゲッター・セッター
	Type GetType() { return m_Type; }
	void SetVelocity(float vx, float vy) { m_Velocity = { vx,vy }; }
};

