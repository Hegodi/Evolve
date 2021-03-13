#pragma once

#include <string>

namespace GeneticAlgorithm
{
	typedef unsigned char Gen;

	constexpr unsigned int GENES_NODE = 5; // radius, friction, elastic, x, y
	constexpr unsigned int GENES_SPRING = 6; // Length, SpringConstant, Index Node1, Index Node2, DeltaLenght, Period
	constexpr unsigned int MAX_NODES = 20;
	constexpr unsigned int MAX_SPRINGS = 30;
	constexpr unsigned int DNA_LENGTH = MAX_NODES * GENES_NODE + MAX_SPRINGS * GENES_SPRING;
	constexpr unsigned int SIZE_DNA = DNA_LENGTH * sizeof(Gen);

	typedef Gen Dna[DNA_LENGTH];

	extern float ReadGen(Gen gen, float min, float max, unsigned char inputMin = 0, unsigned char inputMax = 255);
	extern bool  ReadGenBool(Gen gen, unsigned char threshold);
	extern bool	 IsValidNodeIndex(int index);
	extern void  InitRandom(Dna& dna);
	extern void	 Save(Dna const& dna, std::string filename);
	extern bool	 Load(Dna& dna, std::string filename);
}
