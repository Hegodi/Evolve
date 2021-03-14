#include "Spring.h"

void CSpring::UpdateRelax()
{
	UpdateForce();
}

void CSpring::UpdateForce() 
{
	Vec2f displacement = m_node1->GetPos() - m_node2->GetPos();
	float length = displacement.length();
	if (length == 0.0f)
	{
		return;
	}

	float deltaLength = m_equilibriumLength - length;
	m_potentialEnergy = deltaLength * deltaLength * m_springConstant;
	Vec2f force = (displacement / length) * deltaLength * m_springConstant;
	m_node1->AddForce(force);
	force.invert();
	m_node2->AddForce(force);
}

CSpringActive::CSpringActive(float springConstant, float length, float deltaLengthFactor, float period) : 
	CSpring(springConstant, length), 
	m_referenceLength(length),
	m_deltaLengthFactor(deltaLengthFactor), 
	m_period(period),
	m_time(0.0f)
{
	m_lengthState = ELengthState::Min;
}

void CSpringActive::Update(float deltaTime) 
{
	m_time -= deltaTime;
	if (m_time <= 0.0f)
	{
		if (m_lengthState == ELengthState::Min)
		{
			m_lengthState = ELengthState::Max;
			m_equilibriumLength = (1.0f + m_deltaLengthFactor) * m_referenceLength;
		}
		else if (m_lengthState == ELengthState::Max)
		{
			m_lengthState = ELengthState::Min;
			m_equilibriumLength = m_referenceLength;
		}

		m_time = m_period;
	}

	UpdateForce();
}

CSpringPasive::CSpringPasive(float springConstant, float length) :
	CSpring(springConstant, length)
{
}

void CSpringPasive::Update(float deltaTime) 
{
	UpdateForce();
}