#pragma once


#include <memory>

#include "olcPixelGameEngine.h"
#include "../World/World.h"


class CGraphics : public olc::PixelGameEngine
{
public:
	CGraphics(CWorld* const& world)
	{
		sAppName = "Spring Robots Simulator";
		m_world = world;
	}

	bool OnUserCreate() override
	{
		m_scale = ScreenHeight() / m_world->GetSize();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override;

private:
	void Render();
	void RenderBackground();
	void RenderWorldInfo();
	void CheckUserInput();
	void FixGraphicPosition(Vec2f& pos);

private:
	float m_scale;
	Vec2f m_offset;
	Vec2f m_offsetTarget;
	CWorld* m_world = nullptr;

	int m_indexNodeSelected = -1;
	CNode const* m_nodeSelected = nullptr;
	int m_indexSpringSelected = -1;
	CSpring const* m_springSelected = nullptr;
};


