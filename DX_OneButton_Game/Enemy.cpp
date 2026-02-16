#include "Enemy.h"

Enemy::Enemy()
{
	fallSpeed = 2.0f;
	m_InvincibleTime = 0;
}

void Enemy::Update()
{
	m_Position.y += fallSpeed;
	if(m_InvincibleTime > 0)
	{
		m_InvincibleTime --;
	}
}
