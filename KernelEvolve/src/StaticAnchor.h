#pragma once


#include <thread>
#include <queue>

#define DLLEXPORT __declspec(dllexport) 

class CTraining;
class CSimulation;
struct STrainingSettings;
struct SGraphicsSimulationSettings;
struct SGraphicsNode;
struct SGraphicsSpring;

class CStaticAnchor
{
public:
	static CTraining ms_training;
};

enum EMessageCode
{
	EMessageCode_Info,
	EMessageCode_InfoHigh,
	EMessageCode_Warning,
	EMessageCode_Error
};

extern "C"
{
	typedef void(__stdcall* ProgressCallback)(int, const char*);
	DLLEXPORT int StartTraining(STrainingSettings settings, ProgressCallback progressCallback);
	DLLEXPORT int AbortTraining();
	DLLEXPORT int NumberEpochs();
	DLLEXPORT int RunGraphicsSimulation(SGraphicsSimulationSettings settings);
	DLLEXPORT int GetNumberNodesInWorld();
	DLLEXPORT int GetNumberSpringsInWorld();
	DLLEXPORT float GetWorldXmin();
	DLLEXPORT float GetWorldXmax();
	DLLEXPORT SGraphicsNode* GetNodesInWorld();
	DLLEXPORT SGraphicsSpring* GetSpringsInWorld();
}

