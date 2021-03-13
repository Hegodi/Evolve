#include "EvolutionaryAlgorithm.h"

#include <algorithm>
#include <iostream>

namespace GeneticAlgorithm
{
	CEvolutionaryAlgorithm::CEvolutionaryAlgorithm(int populationSize, int directPromotion, float mutationProbability, int numberCombats)
		: m_populationSize(populationSize) 
		, m_directPromotion(directPromotion)
		, m_mutationProbability(mutationProbability)
		, m_numberCombats(numberCombats)
	{
		m_population = new CScore[m_populationSize];
		m_parentPopulation = new CScore[m_populationSize];

		float memoryKb = (m_populationSize * (GeneticAlgorithm::SIZE_DNA + sizeof(float)) * 2) / 1024.0f;
		std::cout << "##############################################################################\n";
		std::cout << "#                                                                             \n";
		std::cout << "# Training                                                                    \n";
		std::cout << "#                                                                             \n";
		std::cout << "#-----------------------------------------------------------------------------\n";
		std::cout << "# Memory for DNA: " << GeneticAlgorithm::SIZE_DNA << " bytes\n";
		std::cout << "# Total Memory Required: " << memoryKb << " Kb\n";
		std::cout << "#-----------------------------------------------------------------------------\n";
	}

	CEvolutionaryAlgorithm::~CEvolutionaryAlgorithm()
	{
		delete[] m_population;
		delete[] m_parentPopulation;
	}
	void CEvolutionaryAlgorithm::SetScore(int index, float score) 
	{ 
		m_population[index].SetScore(score); 
		m_isPopulationSorted = false;
	}

	void CEvolutionaryAlgorithm::SetDna(int index, Dna const& dna) 
	{ 
		m_population[index].SetDna(dna); 
		m_isPopulationSorted = false;
	}

	void CEvolutionaryAlgorithm::SortPopulation()
	{
		if (m_isPopulationSorted)
		{
			return;
		}

		auto comparable = [](const void* ptr1, const void* ptr2)
		{
			float score1 = ((CScore*)ptr1)->GetScore();
			float score2 = ((CScore*)ptr2)->GetScore();
			if (score1 > score2)
			{
				return -1;
			}
			else if (score2 > score1)
			{
				return 1;
			}
			return 0;
		};
		std::qsort(m_population, m_populationSize, sizeof(CScore), comparable);
		m_isPopulationSorted = true;
	}

	void CEvolutionaryAlgorithm::NextGeneration()
	{
		SortPopulation();

		CScore* tmp = m_parentPopulation;
		m_parentPopulation = m_population;
		m_population = tmp;

		for (int i = 0; i < m_directPromotion; i++)
		{
			m_population[i].SetDna(m_parentPopulation[i].GetDna());
		}

		for (int i = m_directPromotion; i < m_populationSize; i++)
		{
			int ind1 = SelectParent();
			int ind2 = SelectParent();
			SetChildFromParents(i, ind1, ind2);
		}

		m_isPopulationSorted = false;
	}

	int CEvolutionaryAlgorithm::SelectParent()
	{
		int indBest = rand() % m_populationSize;
		float scoreBest = m_parentPopulation[indBest].GetScore();
		for (int i = 0; i < m_numberCombats; i++)
		{
			int indOponent = rand() % m_populationSize;
			float scoreOponent = m_parentPopulation[indOponent].GetScore();
			if (scoreOponent > scoreBest)
			{
				indBest = indOponent;
			}
		}

		return indBest;
	}

	void CEvolutionaryAlgorithm::SetChildFromParents(int indexChild, int indexParent1, int indexParent2)
	{
		Dna& dnaChild = m_population[indexChild].GetDnaWrite();
		Dna const& dnaParent1 = m_parentPopulation[indexParent1].GetDna();
		Dna const& dnaParent2 = m_parentPopulation[indexParent2].GetDna();

		int crossoverPoint = rand() % DNA_LENGTH;
		for (int i = 0; i < crossoverPoint; i++)
		{
			if (ShouldMutate())
			{
				dnaChild[i] = rand() % 255;
			}
			else
			{
				dnaChild[i] = dnaParent1[i];
			}
		}
		for (int i = crossoverPoint; i<DNA_LENGTH; i++)
		{
			if (ShouldMutate())
			{
				dnaChild[i] = rand() % 255;
			}
			else
			{
				dnaChild[i] = dnaParent2[i];
			}
		}
	}

	bool CEvolutionaryAlgorithm::ShouldMutate()
	{
		float prob = ((float)rand()) / RAND_MAX;
		return prob < m_mutationProbability;
	}
}