#include "Missile.h"

Missile::Missile(Type type)
{
	m_Velocity = { 0.0f,0.0f };
}

void Missile::Update()
{
	//ˆÚ“®
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
}
