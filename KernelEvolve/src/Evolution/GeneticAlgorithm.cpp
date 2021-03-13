
#include <iostream>
#include "GeneticAlgorithm.h"

#include<stdlib.h>

namespace GeneticAlgorithm
{
	float ReadGen(Gen gen, float min, float max, unsigned char inputMin, unsigned char inputMax)
	{
		if (gen < inputMin)
		{
			return min;
		}
		else if (gen > inputMax)
		{
			return max;
		}
		unsigned char input = gen - inputMin;
		float deltaInput = inputMax - inputMin;

		return (input / deltaInput) * (max - min) + min;
	}

	bool ReadGenBool(Gen gen, unsigned char threshold)
	{
		return gen > threshold;
	}

	bool IsValidNodeIndex(int index)
	{
		return index >= 0 && index < MAX_NODES;
	}

	void InitRandom(Dna& dna)
	{
		for (int i = 0; i < DNA_LENGTH; i++)
		{
			dna[i] = rand() % 255;
		}
	}

	bool Load(Dna& dna, std::string filename)
	{
		FILE* fd;
		bool result = true;
		fopen_s(&fd, filename.c_str(), "rb");
		if (fd != nullptr)
		{
			int dnaLength;
			fread(&dnaLength, sizeof(dnaLength), 1, fd);
			if (dnaLength != DNA_LENGTH)
			{
				std::cout << "ERROR while reading file. Dna Lenght does not match " << dnaLength << " != " << DNA_LENGTH << "\n";
				result = false;
			}
			else
			{
				fread(dna, sizeof(Gen), DNA_LENGTH, fd);
				result = true;
			}

			std::cout << "================== READ DNA ====================\n";
			for (int i = 0; i < DNA_LENGTH; i++)
			{
				std::cout << (int)dna[i] << " ";
			}
			std::cout << std::endl;
			std::cout << "=============================================\n";
			fclose(fd);
		}
		else
		{
			std::cout << "ERROR file not found\n";
			result = false;
		}
		return result;
	}

	void Save(Dna const& dna, std::string filename)
	{
		FILE* fd;
		fopen_s(&fd, filename.c_str(), "w");
		if (fd != nullptr)
		{
			int dnaLength = DNA_LENGTH;
			fwrite(&dnaLength, sizeof(dnaLength), 1, fd);
			fwrite(dna, sizeof(Gen), DNA_LENGTH, fd);
			fclose(fd);
		}
		else
		{
			std::cout << "ERROR canot open file for writing\n";
		}
	}
}

