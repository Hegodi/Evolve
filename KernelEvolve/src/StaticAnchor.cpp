#include <iostream>
#include "StaticAnchor.h"
#include "Training.h"

//CTraining* CStaticAnchor::ms_training = new CTraining();
CTraining CStaticAnchor::ms_training;


//////////////////////////////////////////////////////////////////////////////////////
//
//		Training
//
int StartTraining(STrainingSettings settings, ProgressCallback progressCallback)
{
	if (CStaticAnchor::ms_training.IsRunning())
	{
		return -1;
	}

	std::cout << settings.m_populationSize << std::endl;
	std::cout << settings.m_directPromotion << std::endl;
	std::cout << settings.m_mutationRate << std::endl;
	std::cout << settings.m_roundsParentSelection << std::endl;
	std::cout << settings.m_maxEpochs << std::endl;
	std::cout << settings.m_periodSave << std::endl;
	std::cout << settings.m_periodOutput << std::endl;
	std::cout << settings.m_numberThreads << std::endl;

	CStaticAnchor::ms_training.Run(settings, progressCallback);
}

int AbortTraining()
{
	if (!CStaticAnchor::ms_training.IsRunning())
	{
		return -2;
	}

	CStaticAnchor::ms_training.RequestAbort();
	return 0;
}

int NumberEpochs()
{
	return CStaticAnchor::ms_training.GetEpochsDone();
}


//////////////////////////////////////////////////////////////////////////////////////
//
//		Graphic Simulation
//

#define OLC_PGE_APPLICATION
#include "Graphics/GraphicsSimulation.h"

// Override base class with your custom functionality
int RunGraphicsSimulation(SGraphicsSimulationSettings settings)
{
	// World
	CWorld* world = new CWorld();

	GeneticAlgorithm::Dna dna;
	GeneticAlgorithm::Load(dna, settings.m_dnaFilename);
	CRobot robot(dna);
	world->AddRobot(robot);
	// Load Robots
	CGraphicsSimulation game(world);
	if (game.Construct(600, 600, 1, 1))
			game.Start();

	delete world;
	return 0;
}

void SNodeInfo::Init(CNode const* const node)
{
	Vec2f pos = node->GetPos();
	m_posX = pos.x;
	m_posY = pos.y;
	m_radius = node->GetRadius();
	m_elasticCoefficient = node->GetElasticCoefficient();
	m_frictionCoefficient = node->GetFrictionCoefficient();
}

void SSpringInfo::Init(CSpring const* const spring, std::vector<CNode*> const& nodes)
{
	m_length = spring->GetLength();
	m_springConstant = spring->GetSpringConstant();

	CNode* nS = spring->GetNodeStart();
	CNode* nE = spring->GetNodeEnd();
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] == nS)
		{
			m_indNodeStart = i;
		}
		if (nodes[i] == nE)
		{
			m_indNodeEnd = i;
		}
	}

	if (CSpringActive const* const springActive = dynamic_cast<CSpringActive const* const>(spring))
	{
		m_deltaLength = springActive->GetDeltaLengthFactor();
		m_period = springActive->GetPeriod();
	}
}

SRobotInfo* CreateRobotInfo(const char* dnaFilename)
{
	GeneticAlgorithm::Dna dna;
	GeneticAlgorithm::Load(dna, dnaFilename);
	CRobot robot(dna);


	std::vector<CNode*> const nodes = robot.GetNodes();
	std::vector<CSpring*> const springs = robot.GetSprings();

	SRobotInfo* robotInfo = new SRobotInfo();
	robotInfo->m_numNodes = nodes.size();
	robotInfo->m_numSprings = springs.size();

	robotInfo->m_nodes = new SNodeInfo[robotInfo->m_numNodes];

	for (int i = 0; i < nodes.size(); i++)
	{
		CNode const* node = nodes[i];
		robotInfo->m_nodes[i].Init(node);
	}

	robotInfo->m_springs = new SSpringInfo[robotInfo->m_numSprings];
	for (int i = 0; i < springs.size(); i++)
	{
		robotInfo->m_springs[i].Init(springs[i], nodes);
	}

	return robotInfo;
}

void DisposeRobotInfo(SRobotInfo* robotInfo)
{
	if (robotInfo->m_nodes != nullptr)
	{
		delete[] robotInfo->m_nodes;
		robotInfo->m_nodes = nullptr;
	}

	if (robotInfo->m_springs != nullptr)
	{
		delete[] robotInfo->m_springs;
		robotInfo->m_springs = nullptr;
	}

	delete robotInfo;
}
