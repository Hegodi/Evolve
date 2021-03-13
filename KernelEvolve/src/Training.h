#pragma once

#include "Evolution/GeneticAlgorithm.h"
#include "Evolution/EvolutionaryAlgorithm.h"
#include "StaticAnchor.h"

struct STrainingSettings
{
	int m_populationSize = 100;
	int m_directPromotion = 5;
	float m_mutationRate = 0.01f;
	int m_roundsParentSelection = 5;
	int m_maxEpochs = 1000;
	int m_periodSave = 10;
	int m_periodOutput = 1;
	int m_numberThreads = 1;
	int m_randomSeed = 0;
	const char* m_resultsName = nullptr;

	float m_weightDistanceTraveled;
    float m_weightCollisionsWithGround;
    float m_weightCollisionsWithObstacles;
    float m_weightMaxHeight;
    float m_weightAverageHeight;
    float m_weightLoadDistanceTraveled;
    float m_weightLoadCollisionsWithGround;
    float m_numberNodesGoal;
    float m_weightNumberNodes;
    float m_weightReactivity;
};

class CTraining
{
public:
	CTraining() {}
	~CTraining() {}
	void Run(STrainingSettings const& settings, ProgressCallback callback);
	void RequestAbort() { m_abortTraining = true; }
	int GetEpochsDone() const { return m_epochs; }
	bool ShouldAbort() const { return m_abortTraining; }
	bool IsRunning() const { return m_isRunning; }
	void SaveResults(GeneticAlgorithm::CEvolutionaryAlgorithm& evolutionaryAlgorithm);

private:
	void SendMessageEpoch(float bestScore);
	void SendMessageSaved(std::string filename);
	void SendMessage(int msgCode, std::string message);

private:
	char m_message[256];
	ProgressCallback m_progressCallback = nullptr;
	// Flow controll
	int m_epochs = 0;
	std::string m_baseFileName = "";

	STrainingSettings m_settings;

	// Creatures
	int m_minNodes = 4;

	// Process
	bool m_abortTraining = false;
	bool m_isRunning = false;
};


extern void ThreadTrain(CTraining* training, GeneticAlgorithm::CEvolutionaryAlgorithm* evolutionaryAlgorithm, STrainingSettings const& settings, int indStart, int indEnd);
