#pragma once
#include "Vector2D.h"

class CObject
{
public:
	void Update(float dt)
	{

	}

	void AddForce(Vec2f force)
	{

	}

private:
	float m_mass;
	float m_radius;
	Vec2f m_pos;
	Vec2f m_force;
};
