#pragma once

#include "Vector2D.h"

constexpr float AIR_FRICTION = 0.999f;
constexpr float AIR_FRICTION_RELAX = 0.80f;

class CNode
{
public:
	CNode();
	CNode(float radius, float friction, float elastic, Vec2f pos, bool isStatic);
	void Init(float radius, float friction, float elastic, Vec2f pos, bool isStatic);
	void ResetForce();
	void AddForce(Vec2f force);
	void Update(float deltaTime);
	void CollisionsWithGround(float xmin);
	void AddOffset(Vec2f const& offset);
	void SetPos(Vec2f const& pos) { m_pos = pos; }

	Vec2f GetPos() const { return m_pos; }
	float GetRadius() const { return m_radius; }
	float GetElasticCoefficient() const { return m_elasticCoefficient; }
	float GetFrictionCoefficient() const { return m_frictionCoefficient; }
	bool  IsColliding(CNode const* const node) const;

	static void ResolveCollisionVelocity(CNode* n1, CNode* n2);
	static bool ResolveCollisionPosition(CNode* n1, CNode* n2);
private:
	Vec2f m_pos;
	Vec2f m_vel;
	float m_radius;
	Vec2f m_force;
	float m_elasticCoefficient;
	float m_frictionCoefficient;
	bool  m_isStatic;
};
