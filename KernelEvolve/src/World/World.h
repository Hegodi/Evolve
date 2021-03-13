#pragma once

#include <vector>
#include <memory>
#include "../Physics/Spring.h"
#include "../Physics/Node.h"
#include "../Physics/Vector2D.h"
#include "Robot.h"

class CWorld
{
public:
	static Vec2f ms_gravity;
	CWorld();
	~CWorld();

	void Update();

	void AddNode(CNode* node) { m_nodes.push_back(node); }
	void AddSpring(CSpring* spring) { m_springs.push_back(spring); }

	std::vector<CNode*> const& GetNodes() { return m_nodes; }
	std::vector<CSpring*> const& GetSprings() { return m_springs; }

	float GetSize() const { return m_size; }
	void AddRobot(CRobot const& robot);
	void AddRobot(CRobot const* robot);
	void Clean();
	Vec2f GetCenter() const;
	float GetXmin() const;
	float GetXmax() const;
	float GetTime() const { return m_time; }
	float GetDeltaTime() const { return m_deltaTime; }

	int GetNumberNodes() const { return m_nodes.size(); }
	int GetNumberSrings() const { return m_springs.size(); }
	CNode const* const GetNode(int index) const{ return m_nodes[index]; }
	CSpring const* const GetSpring(int index) const{ return m_springs[index]; }

private:
	void CollisionsWithGround();

private:
	const float m_deltaTime = 0.005f;
	float m_time = 0.0f;
	bool m_isReady = false;
	float m_size = 150.0f;
	std::vector<CNode*> m_nodes;
	std::vector<CSpring*> m_springs;

};

