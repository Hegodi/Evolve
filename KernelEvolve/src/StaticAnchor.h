#pragma once


#include <thread>
#include <queue>

#define DLLEXPORT __declspec(dllexport) 

class CTraining;
struct STrainingSettings;
struct SGraphicsSimulationSettings;
class CNode;
class CSpring;

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

struct SNodeInfo
{
	void Init(CNode const* const node);
    float m_posX = 0.0f;
    float m_posY = 0.0f;
    float m_radius = 0.0f;
    float m_elasticCoefficient = 0.0f;
    float m_frictionCoefficient = 0.0f;
};

struct SSpringInfo
{
	void Init(CSpring const* const spring, std::vector<CNode*> const& nodes);
    int m_indNodeStart = -1;
    int m_indNodeEnd = -1;
    float m_length = 0.0f;
	float m_springConstant = 0.0f;
    float m_deltaLength = 0.0f;
    float m_period = 0.0f;
};

struct SRobotInfo
{
	int m_numNodes = 0;
	int m_numSprings = 0;
	SNodeInfo* m_nodes = nullptr;
	SSpringInfo* m_springs = nullptr;
};

extern "C"
{
	typedef void(__stdcall* ProgressCallback)(int, const char*);
	DLLEXPORT int StartTraining(STrainingSettings settings, ProgressCallback progressCallback);
	DLLEXPORT int AbortTraining();
	DLLEXPORT int NumberEpochs();

	DLLEXPORT int RunGraphicsSimulation(SGraphicsSimulationSettings settings);
    DLLEXPORT SRobotInfo* CreateRobotInfo(const char* dnaFilename);
    DLLEXPORT void DisposeRobotInfo(SRobotInfo* robotInfo);
}

