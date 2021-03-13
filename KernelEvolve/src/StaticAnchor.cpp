#include <iostream>
#include "StaticAnchor.h"
#include "Training.h"
#include "Simulation.h"

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
