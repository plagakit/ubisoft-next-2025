#pragma once

#include "gui/tween/easing.h"

class ITween
{
public:
	virtual void Update(float dt) = 0;
	virtual bool IsDone() const = 0;
};

template<typename T>
class Tween : public ITween
{
public:
	Tween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type);
	void Update(float dt) override;
	bool IsDone() const override;

private:
	float m_duration;
	T m_start;
	T m_end;
	T& m_value;

	float m_t;
	float m_tStep;
	bool m_done;

	EasingMode m_mode;
	EasingType m_type;
	EasingFunction m_easing;
};

template<typename T>
inline Tween<T>::Tween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type) :
	m_start(start),
	m_end(end),
	m_value(value),
	m_duration(duration),
	m_t(0.0f),
	m_tStep(1.0f / duration),
	m_done(false),
	m_mode(mode),
	m_type(type),
	m_easing(Easing::GetFunc(mode, type))
{}

template<typename T>
void inline Tween<T>::Update(float dt)
{
	if (m_done) 
		return;

	m_t += dt * m_tStep;
	m_value = m_start + (m_end - m_start) * m_easing(m_t);

	if (m_t >= 1.0f)
	{
		m_done = true;
		m_value = m_end;
	}	
}

template<typename T>
inline bool Tween<T>::IsDone() const
{
	return m_done;
}
