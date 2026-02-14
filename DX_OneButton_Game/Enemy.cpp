#include "Enemy.h"

Enemy::Enemy()
{
	fallSpeed = 2.0f;
}

void Enemy::Update()
{
	m_Position.y += fallSpeed;
}
