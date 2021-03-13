#include "ScoreFunctions.h"

float EvaluateCreature(CRobot const& robot, float simulationTime)
{
	if (!robot.IsValid())
	{
		return -99999.0f;
	}

	CWorld world;
	world.AddRobot(robot);

	Vec2f pos0 = robot.GetPos();

	float maxY = 1.0f;
	float dt = world.GetDeltaTime();

	int count = 0;
	float time = 0.0f;
	while (time < simulationTime)
	{
		time += dt;
		world.Update();

		//float yPos = world.GetCenter().y;
		//if (yPos > maxY)
		//{
			//maxY = yPos;
		//}
		count++;
	}

	Vec2f pos1 = robot.GetPos();
	//if (maxY > 10.0f)
	//{
		//return 0.0f;
	//}
	float score = (pos1.x - pos0.x);

	return score;
}
