#include "Node.h"

#include <iostream>

CNode::CNode() :
	m_vel(0.0f),
	m_radius(0.0f),
	m_elasticCoefficient(1.0f),
	m_frictionCoefficient(1.0f),
	m_isStatic(false)
{ }

CNode::CNode(float radius, float friction, float elastic, Vec2f pos, bool isStatic) : 
	m_pos(pos), 
	m_vel(0.0f),
	m_radius(radius),
	m_elasticCoefficient(elastic),
	m_frictionCoefficient(friction),
	m_isStatic(isStatic)
{}

void CNode::Init(float radius, float friction, float elastic, Vec2f pos, bool isStatic)
{
	m_pos = pos;
	m_radius = radius;
	m_elasticCoefficient = elastic;
	m_frictionCoefficient = friction;
	m_isStatic = isStatic;
}

void CNode::ResetForce()
{
	m_force.x = 0.0f;
	m_force.y = 0.0f;
}

void CNode::AddForce(Vec2f force)
{
	if (m_isStatic)
	{
		return;
	}

	m_force += force;
}

void CNode::Update(float deltaTime)
{
	if (m_isStatic)
	{
		return;
	}
	// Warning: it assumes that mass = 1
	m_vel += m_force * deltaTime;
	m_vel *= AIR_FRICTION;
	m_pos += m_vel * deltaTime;
}

void CNode::CollisionsWithGround(float ymin)
{
	if (m_isStatic)
	{
		return;
	}

	if (m_pos.y - m_radius < ymin)
	{
		m_pos.y = ymin + m_radius;
		m_vel.y *= -m_elasticCoefficient;
		m_vel.x *= m_frictionCoefficient;
	}
}

bool CNode::IsColliding(CNode const* const node) const
{
	Vec2f dst = m_pos - node->m_pos;
	float minDst = m_radius + node->m_radius;
	return dst.lengthSqr() < minDst * minDst;
}

void CNode::AddOffset(Vec2f const& offset)
{
	m_pos += offset;
}

// Warning: it assumes that mass = 1
void CNode::ResolveCollisionVelocity(CNode* n1, CNode* n2)
{
	if (n1 == nullptr || n2 == nullptr)
	{
		return;
	}

	Vec2f normal = n1->m_pos - n2->m_pos;
	float distance = normal.length();
	normal /= distance;
	Vec2f tangent = normal.perpendicular();

	float elasticCoef = n1->m_elasticCoefficient * n2->m_elasticCoefficient;
	float friction = n1->m_frictionCoefficient * n2->m_frictionCoefficient;

	float vel2tan = Vec2f::dotProduct(n2->m_vel, tangent);
	float vel2nor = Vec2f::dotProduct(n2->m_vel, normal);
	if (n1->m_isStatic)
	{
		n2->m_vel = tangent * vel2tan * friction - normal * vel2nor * elasticCoef;
		return;
	}

	float vel1tan = Vec2f::dotProduct(n1->m_vel, tangent);
	float vel1nor = Vec2f::dotProduct(n1->m_vel, normal);
	if (n2->m_isStatic)
	{
		n1->m_vel = tangent * vel1tan * friction - normal * vel1nor * elasticCoef;
		std::cout << "=================================\n\n\n\n";
		return;
	}

	n2->m_vel = tangent * vel2tan * friction + normal * vel1nor * elasticCoef;
	n1->m_vel = tangent * vel1tan * friction + normal * vel2nor * elasticCoef;
}

bool CNode::ResolveCollisionPosition(CNode* n1, CNode* n2)
{
	if (n1 == nullptr || n2 == nullptr)
	{
		return false;
	}

	Vec2f tangent = n1->m_pos - n2->m_pos;
	float distance = tangent.length();
	tangent /= distance;
	float minDistance = n1->m_radius + n2->m_radius;
	if (distance < minDistance)
	{
		float deltaPosHalf = (distance - minDistance) * 0.51f;
		if (n1->m_isStatic)
		{
			n2->m_pos += tangent * deltaPosHalf * 2.0f;
		}
		else if (n2->m_isStatic)
		{
			n1->m_pos -= tangent * deltaPosHalf * 2.0f;
		}
		else
		{
			n1->m_pos -= tangent * deltaPosHalf;
			n2->m_pos += tangent * deltaPosHalf;

		}
		return true;
	}
	return false;
}
/*
bool CNode::ResolveCollisionPosition(CNode* n1, CNode* n2)
{
	if (n1 == nullptr || n2 == nullptr)
	{
		return false;
	}

	Vec2f tangent = n1->m_pos - n2->m_pos;
	float distance = tangent.length();
	tangent /= distance;
	float minDistance = n1->m_radius + n2->m_radius;
	if (distance < minDistance)
	{

		float deltaPosHalf = (distance - minDistance) * 0.5f;
		if (n1->m_isStatic)
		{
			n2->m_pos += tangent * deltaPosHalf * 2.1f;
		}
		else if (n2->m_isStatic)
		{
			n1->m_pos -= tangent * deltaPosHalf * 2.1f;
		}
		else
		{
			n1->m_pos += tangent * deltaPosHalf;
			n2->m_pos -= tangent * deltaPosHalf;

		}
		return true;
	}
	return false;
}
*/
