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

public:

	Missile(Type type);
	~Missile() {};

	void Update();

	//ゲッター・セッター
	Type GetType() { return m_Type; }
	void SetVelocity(float vx, float vy) { m_Velocity = { vx,vy }; }
};

