#pragma once

#include "World/World.h"
#include "World/Robot.h"

struct STrainingSettings;

float EvaluateCreature(CRobot const& robot, STrainingSettings const& settings, float simulationTime = 100.0f);

