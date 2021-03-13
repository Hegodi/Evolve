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

	int count = 0;
	float time = 0.0f;
	while (time < simulationTime)
	{
		time += dt;
		world.Update();

		float yPos = world.GetCenter().y;
		if (yPos > maxY)
		{
			maxY = yPos;
		}
		avgY += yPos;
		count++;
	}

	if (count > 0)
	{
		avgY /= count;
	}
	Vec2f pos1 = robot.GetPos();

	float score = 0;
	score += (pos1.x - pos0.x) * settings.m_weightDistanceTraveled;
	score += world.GetNumberCollisionsWithGround() * settings.m_weightCollisionsWithGround / robot.GetNumberNodes();
	score += world.GetNumberCollisionsWithObstacles() * settings.m_weightCollisionsWithObstacles / robot.GetNumberNodes();
	score += maxY * settings.m_weightMaxHeight;
	score += avgY * settings.m_weightAverageHeight;
	// Load Distance traveled
	// Load collisions with ground
	int deltaNodes = (robot.GetNumberNodes() - settings.m_numberNodesGoal);
	score -= deltaNodes * deltaNodes * settings.m_weightNumberNodes;
	score += robot.ComputeReactivity() * settings.m_weightReactivity;

	return score;
}
