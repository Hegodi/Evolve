#pragma once

#include <vector>
#include <string>
#include "../Evolution/GeneticAlgorithm.h"
#include "../Physics/Node.h"
#include "../Physics/Spring.h"

using namespace GeneticAlgorithm;

class CRobot
{
public:
	CRobot(Dna const& dna);
	CRobot();
	~CRobot();

	void Init(Dna const& dna);
	void Clean();
	std::vector<CNode*> const& GetNodes() const { return m_nodes; }
	std::vector<CSpring*> const& GetSprings() const { return m_springs; }
	bool IsValid() const;
	int GetNumberNodes() const;
	Vec2f GetPos() const;
	float ComputeReactivity() const;

	void PrintDebug(std::string txt = "") const;
	void SetScore(float score) { m_score = score; }
	void SetPosition(Vec2f const& pos);
	void SetPositionX(float x);
	void SetPositionY(float y);

private:
	void CreateNodesAndSprings(Dna const& dna);
	void CleanDisconectedParts();


	// Check disconected parts
	void RemoveSpringsNode(CNode* node);
	void RemoveNode(CNode* node);
	std::vector<CNode*> FindLinks(CNode* node);
	void RelaxPosition(float dt, float duration);
	bool IsDuplicate(CSpring* spring);
	// Check duplicates springs


private:
	float const ROBOT_SIZE_MAX = 100.0f;
	float const NODE_RADIUS = 1.0f;
	Dna m_dna;
	std::vector<CNode*> m_nodes;
	std::vector<CSpring*> m_springs;

	float m_score = -1;
};

