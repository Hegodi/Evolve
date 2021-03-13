
#include <vector>
#include <string>
#include "GraphicsSimulation.h"


bool CGraphicsSimulation::OnUserUpdate(float fElapsedTime) 
{
	m_world->Update();
	m_offsetTarget = m_world->GetCenter() * m_scale;
	m_offset = Vector2D<float>::Lerp(m_offset, m_offsetTarget, 0.01f);

	CheckUserInput();
	Render();
	return true;
}

void CGraphicsSimulation::CheckUserInput()
{
	if (GetKey(olc::Key::K1).bPressed)
	{
		m_scale = ScreenHeight() / m_world->GetSize();
	}
	else if (GetKey(olc::Key::K2).bPressed)
	{
		m_scale = 2.0 * ScreenHeight() / m_world->GetSize();
	}
	else if (GetKey(olc::Key::K3).bPressed)
	{
		m_scale = 4.0 * ScreenHeight() / m_world->GetSize();
	}
	else if (GetKey(olc::Key::N).bPressed)
	{
		m_indexNodeSelected = (m_indexNodeSelected + 1) % m_world->GetNumberNodes();
		m_nodeSelected = m_world->GetNode(m_indexNodeSelected);
	}
	else if (GetKey(olc::Key::S).bPressed)
	{
		m_indexSpringSelected = (m_indexSpringSelected + 1) % m_world->GetNumberSrings();
		m_springSelected = m_world->GetSpring(m_indexSpringSelected);
	}
}

void CGraphicsSimulation::Render()
{
	Clear(olc::BLACK);
	RenderBackground();
	RenderWorldInfo();

	if (m_world == nullptr)
	{
		return;
	}

	std::vector<CNode*> const& nodes = m_world->GetNodes();
	for (CNode* const node : nodes)
	{
		olc::Pixel color = olc::RED;
		if (node == m_nodeSelected)
		{
			color = olc::YELLOW;
		}
		Vec2f pos = node->GetPos() * m_scale;
		FixGraphicPosition(pos);
		FillCircle(pos.x, pos.y, node->GetRadius() * m_scale, color);
	}

	std::vector<CSpring*> const& springs = m_world->GetSprings();
	for (CSpring* const spring : springs)
	{
		olc::Pixel color = olc::RED;
		Vec2f pos1 = spring->GetPosStart() * m_scale;
		FixGraphicPosition(pos1);
		Vec2f pos2 = spring->GetPosEnd() * m_scale;
		FixGraphicPosition(pos2);
		if (spring == m_springSelected)
		{
			color = olc::YELLOW;
		}
		else
		{
			if (typeid(*spring) == typeid(CSpringActive))
			{
				color = olc::GREEN;
			}
			else if (typeid(*spring) == typeid(CSpringPasive))
			{
				color = olc::RED;
			}
		}
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, color);
	}
}

void CGraphicsSimulation::RenderBackground()
{
	float lenght = ScreenHeight();
	float deltaX = 10.0f * m_scale;
	int indCenter = (int)(m_offset.x / deltaX);
	for (int i = -8; i < 8; i++)
	{
		float x = (indCenter + i) * deltaX;
		Vec2f pos1;
		pos1.x = x;
		pos1.y = 0.0f;
		Vec2f pos2;
		pos2.x = x;
		pos2.y = lenght;
		FixGraphicPosition(pos1);
		FixGraphicPosition(pos2);
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, olc::GREY);

		Vec2f posText;
		posText.x = x - 10;
		posText.y = -30;
		FixGraphicPosition(posText);
		std::string textNumber = std::to_string((int)(x / m_scale));
		DrawString(posText.x, posText.y, textNumber, olc::GREY);
	}

	Vec2f pos;
	FixGraphicPosition(pos);
	FillRect(0.0, pos.y, ScreenWidth(), 10, olc::WHITE);

	float time = m_world->GetTime();
	std::string textTime = std::to_string(time);
	DrawString(ScreenWidth() - 100, 10, textTime, olc::GREY);
}

void CGraphicsSimulation::RenderWorldInfo()
{
	if (m_nodeSelected != nullptr)
	{
		std::string text;
		text = "Node " + std::to_string(m_indexNodeSelected);
		DrawString(5, 5, text);
		text = "Radius " + std::to_string(m_nodeSelected->GetRadius());
		DrawString(5, 15, text);
		text = "Elastic Coef. " + std::to_string(m_nodeSelected->GetElasticCoefficient());
		DrawString(5, 35, text);
		text = "Friction Coef. " + std::to_string(m_nodeSelected->GetFrictionCoefficient());
		DrawString(5, 45, text);
	}

	if (m_springSelected != nullptr)
	{
		std::string text;
		text = "Spring " + std::to_string(m_indexSpringSelected);
		DrawString(5, 70, text);
		text = "Spring Constant " + std::to_string(m_springSelected->GetSpringConstant());
		DrawString(5, 80, text);
		text = "Length " + std::to_string(m_springSelected->GetLength());
		DrawString(5, 90, text);
		if (CSpringActive const* springActive = dynamic_cast<CSpringActive const*>(m_springSelected))
		{
			text = "Delta Length " + std::to_string(springActive->GetDeltaLengthFactor());
			DrawString(5, 100, text);
			text = "Period " + std::to_string(springActive->GetPeriod());
			DrawString(5, 110, text);
		}

	}
}

void CGraphicsSimulation::FixGraphicPosition(Vec2f& pos)
{
	pos.y = 0.8f * ScreenHeight() - pos.y;
	pos.x += ScreenWidth() * 0.5f;
	pos.x -= m_offset.x;
}
