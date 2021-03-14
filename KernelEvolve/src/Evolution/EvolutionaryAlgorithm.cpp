#include "EvolutionaryAlgorithm.h"

#include <algorithm>
#include <iostream>

namespace GeneticAlgorithm
{
	CEvolutionaryAlgorithm::CEvolutionaryAlgorithm(int populationSize, int directPromotion, float mutationProbability, int numberCombats, float invertCrossoverProb)
		: m_populationSize(populationSize) 
		, m_directPromotion(directPromotion)
		, m_mutationProbability(mutationProbability)
		, m_numberCombats(numberCombats)
		, m_invertCrossoverProbability(invertCrossoverProb)
	{
		m_population = new CScore[m_populationSize];
		m_parentPopulation = new CScore[m_populationSize];

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
		int indParent = 0;
		int deltaIndParent = 1;
		float probInvert = rand() / RAND_MAX;
		if (probInvert < m_invertCrossoverProbability)
		{
			indParent = crossoverPoint-1;
			deltaIndParent = -1;
		}
		for (int i = 0; i < crossoverPoint; i++, indParent += deltaIndParent)
		{
			if (ShouldMutate())
			{
				dnaChild[i] = rand() % 255;
			}
			else
			{
				dnaChild[i] = dnaParent1[indParent];
			}
		}

		indParent = crossoverPoint;
		deltaIndParent = 1;
		if (probInvert < m_invertCrossoverProbability)
		{
			indParent = DNA_LENGTH - 1;
			deltaIndParent = -1;
		}
		for (int i = crossoverPoint; i<DNA_LENGTH; i++, indParent += deltaIndParent)
		{
			if (ShouldMutate())
			{
				dnaChild[i] = rand() % 255;
			}
			else
			{
				dnaChild[i] = dnaParent2[indParent];
			}
		}
	}

	bool CEvolutionaryAlgorithm::ShouldMutate()
	{
		float prob = ((float)rand()) / RAND_MAX;
		return prob < m_mutationProbability;
	}
}