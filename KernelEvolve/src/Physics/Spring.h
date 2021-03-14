#pragma once

#include "Node.h"
class CSpring
{
public:
	CSpring() {}
	CSpring(float springConstant, float lenght) : m_springConstant(springConstant), m_equilibriumLength(lenght) {}

	void Init(float springConstant, float lenght)
	{
		m_springConstant = springConstant;
		m_equilibriumLength = lenght;
	}

	void Connect(CNode* const n1, CNode* const n2)
	{
		m_node1 = n1;
		m_node2 = n2;
	}

	virtual void Update(float deltaTime) = 0;
	void UpdateRelax();
	Vec2f GetPosStart() const { return m_node1->GetPos(); }
	Vec2f GetPosEnd() const { return m_node2->GetPos(); }
	CNode* GetNodeStart() const { return m_node1; }
	CNode* GetNodeEnd() const { return m_node2; }
	virtual float GetLength() const { return m_equilibriumLength; }
	float GetSpringConstant() const { return m_springConstant; }
	float GetPotentialEnergy() const { return m_potentialEnergy; }

protected:
	void UpdateForce();

protected:
	float m_springConstant = 1.0f;
	float m_equilibriumLength = 1.0f;
	CNode* m_node1 = nullptr;
	CNode* m_node2 = nullptr;
	float m_potentialEnergy = 0.0f;
};

class CSpringActive : public CSpring
{
public:
	CSpringActive(float springConstant, float length, float deltaLengthFactor, float period);
	virtual ~CSpringActive() {}
	void Update(float deltaTime) override;

	virtual float GetLength() const override { return m_referenceLength; }

	float GetDeltaLengthFactor() const { return m_deltaLengthFactor; }
	float GetPeriod() const { return m_period; }

private:
	enum class ELengthState {Min, Max};
	ELengthState m_lengthState = ELengthState::Min;
	float m_deltaLengthFactor = 0.0f;	// Lenght variation (0.0f, 1.0f) from the equilirbium length
	float m_referenceLength = 0.0f;
	float m_period = 0.0f;
	float m_time = 0.0f;
};

class CSpringPasive : public CSpring
{
public:
	CSpringPasive(float springConstant, float length);
	virtual ~CSpringPasive() {}
	virtual void Update(float deltaTime) override;
};
