#include <iostream>
#include <memory>

#include "../KernelEvolve/src/World/World.h"
#include "../KernelEvolve/src/World/Robot.h"



#define OLC_PGE_APPLICATION

#include "Graphics.h"

// Override base class with your custom functionality
int main()
{
	// World
	CWorld* world = new CWorld();

	CNode* n1 = new CNode(4.0f, 1.0f, 1.0f, Vec2f(0.0f, 8.0f), false);
	CNode* n2 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(-8.0f, 2.5f), false);
	CNode* n3 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(8.0f, 2.5f), false);
	CNode* n4 = new CNode(2.0f, 1.0f, 1.0f, Vec2f(0.0f, 20.0f), false);

	CSpring* s1 = new CSpringPasive(1.0f, 14.0f);
	CSpring* s2 = new CSpringPasive(1.0f, 4.0f);
	CSpring* s3 = new CSpringPasive(1.0f, 14.0f);

	s1->Connect(n1, n2);
	s2->Connect(n2, n3);
	s3->Connect(n3, n1);
	
	world->AddNode(n1);
	world->AddNode(n2);
	world->AddNode(n3);
	world->AddNode(n4);
	world->AddSpring(s1);
	world->AddSpring(s2);
	world->AddSpring(s3);

	// Load Robots


	//CGraphics game(world);
	//if (game.Construct(600, 600, 1, 1))
			//game.Start();

	for (int i = 0; i < 1000000; i++)
	{
		world->Update();
	}

	delete n1;
	delete n2;
	delete world;

	return 0;
}