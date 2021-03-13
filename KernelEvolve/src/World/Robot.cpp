#include <iostream>

#include "World.h"
#include "Robot.h"
#include "../Evolution/GeneticAlgorithm.h"

constexpr float NodeMassMin = 1.0f;
constexpr float NodeMassMax = 5.0f;
constexpr float NodeRadiusMin = 1.0f;
constexpr float NodeRadiusMax = 2.0f;
constexpr float SpringConstantMin = 0.1f;
constexpr float SpringConstantMax = 2.0f;
constexpr float SpringLengthMin = 10.0f;
constexpr float SpringLengthMax = 40.0f;
constexpr float SpringDeltaLengthMin = 0.1f;
constexpr float SpringDeltaLengthMax = 0.5f;


CRobot::CRobot()
{
}

CRobot::CRobot(Dna const& dna)
{
	Init(dna);
}

void CRobot::Init(Dna const& dna)
{
	Clean();
	CreateNodesAndSprings(dna);
	CleanDisconectedParts();
	RelaxPosition(0.01f, 50.0f);
	SetPositionX(0.0f);
}

void CRobot::CreateNodesAndSprings(Dna const& dna)
{
	for (int i = 0; i < DNA_LENGTH; i++)
	{
		m_dna[i] = dna[i];
	}

	CNode* nodesTmp = new CNode[MAX_NODES];
	int nodesNewIndices[MAX_NODES];

	int ind = 0;

	// mass, radius, friction, elastic, x, y
	for (int i = 0; i < MAX_NODES; i++, ind++)
	{
		float radius = ReadGen(m_dna[ind], NodeRadiusMin, NodeRadiusMax);
		float friction = ReadGen(m_dna[ind + 1], 0.1f, 1.0f);
		float elastic = ReadGen(m_dna[ind + 2], 0.1f, 1.0f);
		float x = ReadGen(m_dna[ind + 3], 0.0f, ROBOT_SIZE_MAX);
		float y = ReadGen(m_dna[ind + 4], 0.0f, ROBOT_SIZE_MAX);
		nodesTmp[i].Init(radius, friction, elastic, Vec2f(x, y), false);
		nodesNewIndices[i] = -1;
	}

	//Active, Length, SpringConstant, Index Node1, Index Node2, DeltaLenght, Period
	for (int i = 0; i < MAX_SPRINGS; i++, ind++)
	{
		bool active = ReadGenBool(m_dna[ind], 128);
		{
			float length		 = ReadGen(m_dna[ind], SpringLengthMin, SpringLengthMax);
			float springConstant = ReadGen(m_dna[ind + 1], SpringConstantMin, SpringConstantMax);
			int	  indexNode1	 = (int)m_dna[ind + 2];
			int	  indexNode2	 = (int)m_dna[ind + 3];
			if ((!IsValidNodeIndex(indexNode1) || !IsValidNodeIndex(indexNode2)) || indexNode1 == indexNode2)
			{
				continue;
			}
			
			if (nodesNewIndices[indexNode1] < 0)
			{
				nodesNewIndices[indexNode1] = m_nodes.size();
				m_nodes.push_back(new CNode(nodesTmp[indexNode1]));
			}
			if (nodesNewIndices[indexNode2] < 0)
			{
				nodesNewIndices[indexNode2] = m_nodes.size();
				m_nodes.push_back(new CNode(nodesTmp[indexNode2]));
			}

			int indexNew1 = nodesNewIndices[indexNode1];
			int indexNew2 = nodesNewIndices[indexNode2];

			float deltaLength = ReadGen(m_dna[ind + 4], 0.0f, SpringDeltaLengthMax);
			CSpring* spring = nullptr;
			if (deltaLength < SpringDeltaLengthMin)
			{
				spring = new CSpringPasive(springConstant, length);
				spring->Connect(m_nodes[indexNew1], m_nodes[indexNew2]);
			}
			else
			{
				float period = ReadGen(m_dna[ind + 5], 0.5f, 2.0f);
				spring = new CSpringActive(springConstant, length, deltaLength, period);
				spring->Connect(m_nodes[indexNew1], m_nodes[indexNew2]);
			}

			if (spring != nullptr && !IsDuplicate(spring))
			{
				m_springs.push_back(spring);
			}
		}
	}

	delete[] nodesTmp;
}

void CRobot::CleanDisconectedParts()
{
	if (m_nodes.size() == 0)
	{
		return;
	}

	std::vector<CNode*> m_openList;
	std::vector<CNode*> m_unusedList(m_nodes);
	std::vector<CNode*> m_usedList;
	m_openList.push_back(m_nodes[0]);

	while (m_openList.size() > 0)
	{
		CNode* node = m_openList[m_openList.size()-1];
		m_openList.pop_back();

		std::vector<CNode*> linkedNodes = FindLinks(node);

		m_usedList.push_back(node);
		auto it = find(m_unusedList.begin(), m_unusedList.end(), node);
		if (it != m_unusedList.end())
		{
			m_unusedList.erase(it);
		}


		for (CNode* linkedNode : linkedNodes)
		{
			if (std::find(m_usedList.begin(), m_usedList.end(), linkedNode) == m_usedList.end())
			{
				m_openList.push_back(linkedNode);
			}
		}
	}

	for (int i = 0; i < m_unusedList.size(); i++)
	{
		CNode* node = m_unusedList[i];
		RemoveSpringsNode(node);
	}
	for (int i = 0; i < m_unusedList.size(); i++)
	{
		CNode* node = m_unusedList[i];
		RemoveNode(node);
	}
}

CRobot::~CRobot()
{
	Clean();
}

void CRobot::Clean()
{
	for (CNode* node : m_nodes)
	{
		delete node;
	}

	for (CSpring* spring : m_springs)
	{
		delete spring;
	}

	m_nodes.clear();
	m_springs.clear();
}

bool CRobot::IsValid() const
{
	return m_nodes.size() > 1;
}

int CRobot::GetNumberNodes() const
{
	return m_nodes.size();
}

Vec2f CRobot::GetPos() const
{
	Vec2f pos;
	pos.x = 0.0f;
	pos.y = 0.0f;
	for (CNode* node : m_nodes)
	{
		pos += node->GetPos();
	}

	pos /= m_nodes.size();
	return pos;
}

float CRobot::ComputeReactivity() const
{
	float reactivity = 0.0f;
	for (CSpring const* const spring : m_springs)
	{
		if (CSpringActive const* const springActive = dynamic_cast<CSpringActive const* const>(spring))
		{
			float length = springActive->GetLength();
			float deltaFactor = springActive->GetDeltaLengthFactor();
			reactivity += length * length * deltaFactor * deltaFactor;
		}
	}

	return reactivity;
}

void CRobot::PrintDebug(std::string txt) const
{
	std::cout << "============" << txt << "============\n";
	std::cout << "Nodes: " << m_nodes.size() << "\n";
	std::cout << "Springs: " << m_springs.size() << "\n";
	for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		std::cout << (*it)->GetPos().x << "\n";
	}
}

void CRobot::RemoveSpringsNode(CNode* nodeToDelete)
{
	for (auto it = m_springs.begin(); it != m_springs.end(); )
	{
		CSpring* spring = *it;
		if (spring->GetNodeStart() == nodeToDelete)
		{
			it = m_springs.erase(it);
			delete spring;
		}
		else if (spring->GetNodeEnd() == nodeToDelete)
		{
			it = m_springs.erase(it);
			delete spring;
		}
		else
		{
			++it;
		}
	}
}

void CRobot::RemoveNode(CNode* nodeToDelete)
{
	for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		CNode* node = *it;
		if (node == nodeToDelete)
		{
			m_nodes.erase(it);
			delete node;
			return;
		}
	}
}

std::vector<CNode*> CRobot::FindLinks(CNode* node)
{
	std::vector<CNode*> links;
	for (auto it = m_springs.begin(); it != m_springs.end(); ++it)
	{
		CSpring* spring = *it;
		if (spring->GetNodeStart() == node)
		{
			links.push_back(spring->GetNodeEnd());
		}
		else if (spring->GetNodeEnd() == node)
		{
			links.push_back(spring->GetNodeStart());
		}
	}
	return links;
}

void CRobot::RelaxPosition(float dt, float duration)
{
	int cycles = duration / dt;
	for (int i = 0; i < cycles; i++)
	{
		for (CNode* node : m_nodes)
		{
			node->ResetForce();
			node->AddForce(CWorld::ms_gravity);
		}

		for (CSpring* spring : m_springs)
		{
			spring->UpdateRelax();
		}

		for (CNode* node : m_nodes)
		{
			node->Update(dt);
			node->CollisionsWithGround(0.0f);
		}
	}
}

bool CRobot::IsDuplicate(CSpring* spring1)
{
	for (CSpring* spring2 : m_springs)
	{
		if ((spring1->GetNodeStart() == spring2->GetNodeStart() && spring1->GetNodeEnd() == spring2->GetNodeEnd()) ||
			(spring1->GetNodeEnd() == spring2->GetNodeStart() && spring1->GetNodeStart() == spring2->GetNodeEnd()))
		{
			return true;
		}
	}

	return false;
}

void CRobot::SetPosition(Vec2f const& pos)
{
	Vec2f center = GetPos();
	Vec2f offset = pos - center.x;
	for (CNode* node : m_nodes)
	{
		node->AddOffset(offset);
	}
}

void CRobot::SetPositionX(float x)
{
	Vec2f center = GetPos();
	float delta = x - center.x;
	Vec2f offset(delta, 0.0f);
	for (CNode* node : m_nodes)
	{
		node->AddOffset(offset);
	}
}

void CRobot::SetPositionY(float y)
{
	Vec2f center = GetPos();
	float delta = y - center.y;
	Vec2f offset(0.0f, delta);
	for (CNode* node : m_nodes)
	{
		node->AddOffset(offset);
	}
}

