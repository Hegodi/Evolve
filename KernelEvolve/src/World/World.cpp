#include <iostream>

#include "World.h"
#include "../Evolution/GeneticAlgorithm.h"

Vec2f CWorld::ms_gravity = Vec2f(0.0f, -10.0f);

CWorld::CWorld()
{
}

CWorld::~CWorld()
{
	Clean();
}

void CWorld::Update()
{
	m_time += m_deltaTime;
	for (CNode* node : m_nodes)
	{
		node->ResetForce();
		node->AddForce(ms_gravity);
	}

	for (CSpring* spring : m_springs)
	{
		spring->Update(m_deltaTime);
	}

	// Movement
	for (CNode* node : m_nodes)
	{
		node->Update(m_deltaTime);
	}

	// Collisions
	for (int i = 0; i < m_nodes.size(); i++)
	{
		CNode* n1 = m_nodes[i];
		for (int j = i + 1; j < m_nodes.size(); j++)
		{
			CNode* n2 = m_nodes[j];
			if (n1->IsColliding(n2))
			{
				CNode::ResolveCollisionVelocity(n1, n2);
			}
		}
	}

	bool anyCollision = true;
	int attempts = 0;
	while (anyCollision && attempts < 5)
	{
		CollisionsWithGround();
		for (int i = 0; i < m_nodes.size(); i++)
		{
			anyCollision = false;
			CNode* n1 = m_nodes[i];
			for (int j = i + 1; j < m_nodes.size(); j++)
			{
				CNode* n2 = m_nodes[j];
				if (n1->IsColliding(n2))
				{
					CNode::ResolveCollisionPosition(n1, n2);
					anyCollision = true;
				}
			}
			attempts++;
		}
	}
}

void CWorld::CollisionsWithGround()
{
	for (CNode* node : m_nodes)
	{
		node->CollisionsWithGround(0.0f);
	}
}

void CWorld::AddRobot(CRobot const* robot)
{
	if (robot != nullptr)
	{
		AddRobot(*robot);
	}
}

void CWorld::AddRobot(CRobot const& robot)
{
	std::vector<CNode*> nodes = robot.GetNodes();
	for (CNode* node : nodes)
	{
		m_nodes.push_back(node);
	}

	std::vector<CSpring*> springs = robot.GetSprings();
	for (CSpring* spring : springs)
	{
		m_springs.push_back(spring);
	}
}

void CWorld::Clean()
{
	m_nodes.clear();
	m_springs.clear();
}

Vec2f CWorld::GetCenter() const
{
	Vec2f center;
	int count = 0;
	for (CNode* node : m_nodes)
	{
		center += node->GetPos();
		count++;
	}

	if (count > 0)
	{
		center /= count;
	}
	return center;
}

float CWorld::GetXmin() const
{
	if (m_nodes.size() == 0)
	{
		return 0;
	}
	float xmin = m_nodes[0]->GetPos().x;
	for (int i=1; i<m_nodes.size(); i++)
	{
		float x = m_nodes[i]->GetPos().x;
		if (x < xmin)
		{
			xmin = x;
		}
	}
	return xmin;
}

float CWorld::GetXmax() const
{
	if (m_nodes.size() == 0)
	{
		return 0;
	}
	float xmax = m_nodes[0]->GetPos().x;
	for (int i=1; i<m_nodes.size(); i++)
	{
		float x = m_nodes[i]->GetPos().x;
		if (x > xmax)
		{
			xmax = x;
		}
	}
	return xmax;
}
