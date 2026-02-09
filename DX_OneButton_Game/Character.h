#pragma once
#include "GameObject.h"

class Character :public GameObject
{
protected:
	DirectX::XMFLOAT2 m_Velocity; // 速度ベクトル
	bool m_IsOnGround;        // 地面に接地しているかどうか

	float m_Gravity;        // 重力加速度
	float m_MaxFallSpeed;   // 最大落下速度
	float m_MoveSpeed;      // 水平移動速度

public:
};

