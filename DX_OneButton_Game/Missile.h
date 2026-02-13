#pragma once
#include "Character.h"

class Missile :public Character
{
public:
	enum class Type
	{
		MAIN,       //最初の一発
		HORIZONTAL, //横に飛ぶ
		VERTICAL,   //縦に飛ぶ
	};
private:
	Type m_Type;
	DirectX::XMFLOAT2 m_Velocity;
	int generation; //分裂回数

public:

	Missile(Type type,int generation = 0);
	~Missile() {};

	void Update();

	//ゲッター・セッター
	Type GetType() { return m_Type; }
	int GetGeneration() { return generation; }
	void SetVelocity(float vx, float vy) { m_Velocity = { vx,vy }; }
};

