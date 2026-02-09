#include "Enemy.h"

Enemy::Enemy()
{
	fallSpeed = 3.0f;
}

void Enemy::Update()
{
	m_Position.y += fallSpeed;
}
