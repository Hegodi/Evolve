#pragma once
#include <vector>
#include <string>
#include "World/World.h"
#include "Physics/Node.h"
#include "Physics/Spring.h"
#include "StaticAnchor.h"

struct SSimulationSettings
{
	const char* m_dnaFilename;
};

struct SGraphicsNode
{
	void Init(CNode const* const n)
	{
		Vec2f pos = n->GetPos();
		m_posX = pos.x;
		m_posY = pos.y;
		m_radius = n->GetRadius();
	}
	float m_posX;
	float m_posY;
	float m_radius;
};

struct SGraphicsSpring
{
	void Init(CSpring const* const s)
	{
		Vec2f p1 = s->GetPosStart();
		Vec2f p2 = s->GetPosEnd();
		m_p1X = p1.x;
		m_p1Y = p1.y;
		m_p2X = p2.x;
		m_p2Y = p2.y;
		m_length = s->GetLength();
	}

	float m_p1X;
	float m_p1Y;
	float m_p2X;
	float m_p2Y;
	float m_length;
};

class CSimulation
{
public:
	int Setup(SSimulationSettings const& settings);
	int Run(int iterations);


	std::vector<SGraphicsNode> const& GetGraphicNodes() const { return m_graphicNodes; }
	std::vector<SGraphicsSpring> const& GetGraphicSprings() const { return m_graphicSprings; }
	float GetWorldXmin() const;
	float GetWorldXmax() const;

private:
	void CreateGraphicObject(CWorld const& world);
	int UpdateGraphicObject(CWorld const& world);

private:
	// Process
	bool m_isInit = false;

	CRobot* m_robot = nullptr;
	SSimulationSettings m_settings;
	CWorld m_world;

	std::vector<SGraphicsNode> m_graphicNodes;
	std::vector<SGraphicsSpring> m_graphicSprings;
};
