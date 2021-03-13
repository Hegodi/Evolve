#include <iostream>
#include "StaticAnchor.h"
#include "Training.h"
#include "Simulation.h"

//CTraining* CStaticAnchor::ms_training = new CTraining();
CTraining CStaticAnchor::ms_training;
CSimulation CStaticAnchor::ms_simulation;


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


int SetupSimulation(SSimulationSettings settings)
{
	CStaticAnchor::ms_simulation.Setup(settings);
	return 0;
}

//int RunSimulation(int iterations)
//{
	//CStaticAnchor::ms_simulation.Run(iterations);
	//return 0;
//}

SGraphicsSpring* GetSpringsInWorld()
{
	std::vector<SGraphicsSpring> springs = CStaticAnchor::ms_simulation.GetGraphicSprings();
	return &springs[0];
}

SGraphicsNode* GetNodesInWorld()
{
	std::vector<SGraphicsNode> nodes = CStaticAnchor::ms_simulation.GetGraphicNodes();
	return &nodes[0];
}

int GetNumberSpringsInWorld()
{
	return CStaticAnchor::ms_simulation.GetGraphicSprings().size();
}

int GetNumberNodesInWorld()
{
	return CStaticAnchor::ms_simulation.GetGraphicNodes().size();
}

float GetWorldXmin()
{
	return CStaticAnchor::ms_simulation.GetWorldXmin();
}

float GetWorldXmax()
{
	return CStaticAnchor::ms_simulation.GetWorldXmax();
}


#define OLC_PGE_APPLICATION
#include "Graphics/Graphics.h"

// Override base class with your custom functionality
int RunSimulation(int iterations)
{
	// World
	CWorld* world = new CWorld();

	CNode* n1 = new CNode(4.0f, 1.0f, 1.0f, Vec2f(0.0f, 8.0f), false);
	CNode* n2 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(-8.0f, 2.5f), false);
	CNode* n3 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(8.0f, 2.5f), false);
	CNode* n4 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(0.0f, 20.0f), false);

	CSpring* s1 = new CSpringPasive(1.0f, 14.0f);
	CSpring* s2 = new CSpringPasive(1.0f, 4.0f);
	CSpring* s3 = new CSpringPasive(1.0f, 14.0f);

	s1->Connect(n1, n2);
	s2->Connect(n2, n3);
	s3->Connect(n3, n1);
	
	world->AddNode(n1);
	world->AddNode(n2);
	world->AddNode(n3);
	world->AddNode(n4);
	world->AddSpring(s1);
	world->AddSpring(s2);
	world->AddSpring(s3);

	// Load Robots
	CGraphics game(world);
	if (game.Construct(600, 600, 1, 1))
			game.Start();

	delete n1;
	delete n2;
	delete world;

	return 0;
}
