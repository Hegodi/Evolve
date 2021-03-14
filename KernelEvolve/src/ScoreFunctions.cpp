#include "ScoreFunctions.h"
#include "Training.h"

#include <iostream>

#define MIN_VALUE -9999999.99

float EvaluateCreature(CRobot const& robot, STrainingSettings const& settings, float simulationTime)
{
	if (!robot.IsValid())
	{
		return MIN_VALUE;
	}

	CWorld world;
	world.AddRobot(robot);

	Vec2f pos0 = robot.GetPos();

	float maxY = MIN_VALUE;
	float avgY = 0.0f;

	float dt = world.GetDeltaTime();

	while (world.GetTime() < simulationTime)
	{
		world.Update();

		float yPos = world.GetCenter().y;
		if (yPos > maxY)
		{
			maxY = yPos;
		}
		avgY += yPos;
	}

	avgY /= world.GetTime();
	Vec2f pos1 = robot.GetPos();

	float score = 0;
	float normalizationFactor = robot.GetNumberNodes() * world.GetTime();
	score += (pos1.x - pos0.x) * settings.m_weightDistanceTraveled;
	score += world.GetNumberCollisionsWithGround() * settings.m_weightCollisionsWithGround / normalizationFactor;
	score += world.GetNumberCollisionsWithObstacles() * settings.m_weightCollisionsWithObstacles / normalizationFactor;
	score += maxY * settings.m_weightMaxHeight;
	score += avgY * settings.m_weightAverageHeight;
	int deltaNodes = (robot.GetNumberNodes() - settings.m_numberNodesGoal);
	score -= deltaNodes * deltaNodes * settings.m_weightNumberNodes;
	score += robot.GetSpringsPotentialEnergy() * settings.m_weightPotentialEnergy;

	return score;
}
