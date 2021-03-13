#include "Training.h";

#include <iostream>
#include <thread>
#include <chrono>

#include "World/World.h"
#include "ScoreFunctions.h"


void CTraining::Run(STrainingSettings const& settings, ProgressCallback progressCallback)
{
	m_progressCallback = progressCallback;
	m_abortTraining = false;
	m_isRunning = true;
	m_settings = settings;
	m_epochs = 0;

	std::cout << "NAME: " << m_settings.m_resultsName << "\n\n";

	m_progressCallback(EMessageCode_Info, "Training Started");

	int testsPerThread = m_settings.m_populationSize / m_settings.m_numberThreads;
	if (m_settings.m_populationSize % m_settings.m_numberThreads != 0)
	{
		m_settings.m_populationSize = testsPerThread * m_settings.m_numberThreads;
	}

	if (m_settings.m_directPromotion > m_settings.m_populationSize)
	{
		m_settings.m_directPromotion = m_settings.m_populationSize;

	}

	CEvolutionaryAlgorithm evolutionaryAlgorithm(m_settings.m_populationSize, m_settings.m_directPromotion, m_settings.m_mutationRate, m_settings.m_roundsParentSelection);

	std::vector<std::thread> threads;

	int minNodes = m_minNodes;
	auto timePoint0 = std::chrono::steady_clock::now();
	SendMessage(EMessageCode_Info, "Initizalizing population ...");
	// Initial population
	for (int i = 0; i < m_settings.m_populationSize; i++)
	{
		if (i % 20 == 0)
		{
			minNodes--;
			if (minNodes < 2)
			{
				minNodes = 2;
			}

		}
		GeneticAlgorithm::Dna dna;
		GeneticAlgorithm::InitRandom(dna);
		CRobot robot(dna);
		int attempt = 0;
		while (robot.GetNumberNodes() < minNodes && attempt < 100)
		{
			GeneticAlgorithm::InitRandom(dna);
			robot.Init(dna);
		}

		float score = EvaluateCreature(robot);
		evolutionaryAlgorithm.SetDna(i, dna);
		evolutionaryAlgorithm.SetScore(i, score);

		if (m_abortTraining)
		{
			break;
		}
	}

	SendMessage(EMessageCode_Info, "Initizalization DONE");
	while (m_epochs < m_settings.m_maxEpochs && !m_abortTraining)
	{

		evolutionaryAlgorithm.NextGeneration();

		threads.clear();
		for (int i = 0; i < m_settings.m_numberThreads; i++)
		{
			int indStart = i * testsPerThread;
			int indEnd = indStart + testsPerThread;
			threads.push_back(std::thread(ThreadTrain, this, &evolutionaryAlgorithm, indStart, indEnd));
		}

		for (int i = 0; i < m_settings.m_numberThreads; i++)
		{
			threads[i].join();
		}


		if (m_epochs % m_settings.m_periodSave == 0 && m_epochs > 0)
		{
			SaveResults(evolutionaryAlgorithm);
		}

		if (m_epochs % m_settings.m_periodOutput == 0 && m_epochs > 0)
		{
			evolutionaryAlgorithm.SortPopulation();
			float bestSocre = evolutionaryAlgorithm.GetScore(0);
			SendMessageEpoch(bestSocre);
		}

		m_epochs++;
	}

	if (m_epochs == m_settings.m_maxEpochs && m_epochs % m_settings.m_periodSave != 0)
	{
		SaveResults(evolutionaryAlgorithm);
	}

	if (m_abortTraining)
	{
		SendMessage(EMessageCode_Aborted, "Training aborted");
	}
	else
	{
		SendMessage(EMessageCode_Completed, "Training completed");
	}
	m_isRunning = false;
	std::cout << "Simulation Finished\n";
}

void CTraining::SaveResults(GeneticAlgorithm::CEvolutionaryAlgorithm& evolutionaryAlgorithm)
{
	evolutionaryAlgorithm.SortPopulation();
	std::string filename = m_settings.m_resultsName + std::to_string(m_epochs) + ".bin";
	GeneticAlgorithm::Save(evolutionaryAlgorithm.GetDna(0), filename);
	SendMessageSaved(filename);
}

void CTraining::SendMessageEpoch(float bestScore)
{
	if (m_progressCallback == nullptr)
	{
		return;
	}

	sprintf_s(m_message, "Epoch %4d    BestScore:%.3f", m_epochs, bestScore);
	m_progressCallback(EMessageCode_Info, m_message);
}

void CTraining::SendMessageSaved(std::string filename)
{
	if (m_progressCallback == nullptr)
	{
		return;
	}

	sprintf_s(m_message, "Results saved (%s)", filename.c_str());
	m_progressCallback(EMessageCode_Saved, m_message);
}

void CTraining::SendMessage(int msgCode, std::string message)
{
	if (m_progressCallback == nullptr)
	{
		return;
	}

	sprintf_s(m_message, "%s", message.c_str());
	m_progressCallback(msgCode, m_message);
}

void ThreadTrain(CTraining* training, CEvolutionaryAlgorithm* evolutionaryAlgorithm, int indStart, int indEnd)
{
	for (int i = indStart; i < indEnd; i++)
	{
		CRobot robot(evolutionaryAlgorithm->GetDna(i));
		float score = EvaluateCreature(robot);
		evolutionaryAlgorithm->SetScore(i, score);

		if (training->ShouldAbort())
		{
			return;
		}
	}
}

