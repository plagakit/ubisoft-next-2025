#pragma once

#include "gui/tween/easing.h"
#include "core/signal/callback.h"
#include "math/math_utils.h"

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
	Tween(const T& start, const T& end, T& value, float duration, EasingMode mode, EasingType type,	Callback<void> callback);
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

	Callback<void> m_callback;
	static void DoNothing() {};

	T Interpolate();
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
{
	m_callback = Callback<void>();
	m_callback.Bind<&DoNothing>();
}

template<typename T>
inline Tween<T>::Tween(const T & start, const T & end, T & value, float duration, EasingMode mode, EasingType type, Callback<void> callback) :
	m_start(start),
	m_end(end),
	m_value(value),
	m_duration(duration),
	m_t(0.0f),
	m_tStep(1.0f / duration),
	m_done(false),
	m_mode(mode),
	m_type(type),
	m_easing(Easing::GetFunc(mode, type)),
	m_callback(callback)
{}

template<typename T>
void inline Tween<T>::Update(float dt)
{
	if (m_done) 
		return;

	m_t += dt * m_tStep;
	m_value = Interpolate();//m_start + (m_end - m_start) * m_easing(m_t);

	if (m_t >= 1.0f)
	{
		m_done = true;
		m_value = m_end;
		m_callback();
	}	
}

template<typename T>
inline bool Tween<T>::IsDone() const
{
	return m_done;
}

// Explicitly specialize Quaternion to use Slerp

template<typename T>
inline T Tween<T>::Interpolate()
{
	return Math::Lerp(m_start, m_end, m_easing(m_t));
}

template<>
inline Quat Tween<Quat>::Interpolate()
{
	return Math::Slerp(m_start, m_end, m_easing(m_t));
}
