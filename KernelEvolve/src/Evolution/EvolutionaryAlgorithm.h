#pragma once

#include "GeneticAlgorithm.h"

namespace GeneticAlgorithm
{

	struct CScore
	{
	public:
		void SetDna(Dna const& dna)
		{
			for (int i = 0; i < DNA_LENGTH; i++)
			{
				m_dna[i] = dna[i];
			}
		}

		void SetScore(float score)
		{
			m_score = score;
		}
		Dna const& GetDna() { return m_dna; }
		Dna& GetDnaWrite() { return m_dna; }
		float GetScore() { return m_score; }

	private:
		Dna m_dna;
		float m_score = -1.0f;
	};

	class CEvolutionaryAlgorithm
	{
	public :
		CEvolutionaryAlgorithm(int populationSize = 100, int directPromotion = 2, float mutationProbability = 0.001, int numberCombats = 1, float invertCrossover = 0.0f);
		~CEvolutionaryAlgorithm();
		Dna const& GetDna(int index) const { return m_population[index].GetDna(); }
		float GetScore(int index) const { return m_population[index].GetScore(); }
		void SetScore(int index, float score);
		void SetDna(int index, Dna const& dna);
		int GetPopulationSize() const { return m_populationSize; }

		void NextGeneration();
		void SortPopulation();

	private:
		int SelectParent();
		void SetChildFromParents(int indexChild, int indexParent1, int indexParent2);
		bool ShouldMutate();


	private:
		float m_mutationProbability = 0.0f;
		float m_invertCrossoverProbability = 0.0f;
		int m_directPromotion = 0;
		int m_numberCombats = 1;



		bool m_isPopulationSorted = false;
		CScore* m_population = nullptr;
		CScore* m_parentPopulation = nullptr;
		int m_populationSize = 0;



	};
}
