#include <stdlib.h>

#include "Simulation.h"

#include "Evolution/GeneticAlgorithm.h"
#include "World/World.h"
#include "World/Robot.h"

int CSimulation::Setup(SSimulationSettings const& settings)
{
	if (m_robot != nullptr)
	{
		delete m_robot;
		m_robot = nullptr;
	}

	m_world.Clean();
	m_isInit = false;
	m_settings = settings;
	GeneticAlgorithm::Dna dna;
	GeneticAlgorithm::Load(dna, m_settings.m_dnaFilename);
	m_robot = new CRobot(dna);
	if (!m_robot->IsValid())
	{
		return -1;
	}
	m_world.AddRobot(*m_robot);
	CreateGraphicObject(m_world);
	UpdateGraphicObject(m_world);
	m_isInit = true;

	return 0;
}

int CSimulation::Run(int iterations)
{
	if (!m_isInit)
	{
		return -1;
	}

	for (int i=0; i<iterations; i++)
	{
		m_world.Update();
	}
	UpdateGraphicObject(m_world);
	return 0;
}

void CSimulation::CreateGraphicObject(CWorld const& world)
{
	m_graphicNodes.clear();
	int numNodes = world.GetNumberNodes();
	for (int i = 0; i < numNodes; i++)
	{
		m_graphicNodes.push_back(SGraphicsNode());
	}

	m_graphicSprings.clear();
	int numSprings = world.GetNumberSrings();
	for (int i = 0; i < numSprings; i++)
	{
		m_graphicSprings.push_back(SGraphicsSpring());
	}
}

int CSimulation::UpdateGraphicObject(CWorld const& world)
{
	int numNodes = world.GetNumberNodes();
	if (numNodes != m_graphicNodes.size())
	{
		return -1;
	}

	for (int i = 0; i < numNodes; i++)
	{
		m_graphicNodes[i].Init(world.GetNode(i));
	}

	int numSprings = world.GetNumberSrings();
	if (numSprings != m_graphicSprings.size())
	{
		return -2;
	}

	for (int i = 0; i < numSprings; i++)
	{
		m_graphicSprings[i].Init(world.GetSpring(i));
	}

	return 0;
}

float CSimulation::GetWorldXmin() const
{
	return m_world.GetXmin();
}

float CSimulation::GetWorldXmax() const
{
	return m_world.GetXmax();
}
