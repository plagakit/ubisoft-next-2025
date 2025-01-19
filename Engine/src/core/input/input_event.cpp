#include "pch.h"
#include "input_event.h"

#include <type_traits>
#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#endif

bool InputEvent::Equals(const InputEvent& event)
{
	// If they are not the same InputEvent type, then return
	// false, but if they are, try the specific Equals implementation.
	// This being virtual ensures we will call the derived EqualsImpl
	return typeid(*this) == typeid(event) 
		&& EqualsImpl(event);
}

bool InputEvent::IsDown() const
{
	return m_isDown;
}

float InputEvent::Strength() const
{
	return m_strength;
}

// VIRTUAL KEY

InputEventVirtual::InputEventVirtual(unsigned char virtualKey) :
	m_virtualKey(virtualKey)
{}

void InputEventVirtual::Update(float dt)
{
#ifdef PLATFORM_WINDOWS
	m_isDown = App::IsKeyPressed(m_virtualKey);
	m_strength = m_isDown ? 1.0f : 0.0f;
#endif
}

bool InputEventVirtual::EqualsImpl(const InputEvent& base) const
{
	const auto& event = static_cast<const InputEventVirtual&>(base);
	return m_virtualKey == event.m_virtualKey;
}

// CONTROLLER TRIGGER

InputEventControllerTrigger::InputEventControllerTrigger(int deviceNum, Type type) :
	m_deviceNum(deviceNum), m_type(type)
{}

void InputEventControllerTrigger::Update(float dt)
{
#ifdef PLATFORM_WINDOWS
	const CController& c = App::GetController(m_deviceNum);
	float lx = c.GetLeftThumbStickX();
	float ly = c.GetLeftThumbStickY();
	float rx = c.GetRightThumbStickX();
	float ry = c.GetRightThumbStickY();

	switch (m_type)
	{
	case THUMB_L_LEFT:	m_strength = lx < 0.0f ? -lx : 0.0f; break;
	case THUMB_L_RIGHT: m_strength = lx > 0.0f ? lx : 0.0f;	 break;
	case THUMB_L_DOWN:	m_strength = ly < 0.0f ? -ly : 0.0f; break;
	case THUMB_L_UP:	m_strength = ly > 0.0f ? ly : 0.0f;	 break;

	case THUMB_R_LEFT:	m_strength = rx < 0.0f ? -rx : 0.0f; break;
	case THUMB_R_RIGHT: m_strength = rx > 0.0f ? rx : 0.0f;	 break;
	case THUMB_R_DOWN:	m_strength = ry < 0.0f ? -ry : 0.0f; break;
	case THUMB_R_UP:	m_strength = ry > 0.0f ? ry : 0.0f;	 break;

	case TRIGGER_L: m_strength = c.GetLeftTrigger();  break;
	case TRIGGER_R: m_strength = c.GetRightTrigger(); break;

	default: m_strength = 0.0f;
	}

	m_isDown = m_strength >= 0.0f;
#endif
}

bool InputEventControllerTrigger::EqualsImpl(const InputEvent& base) const
{
	const auto& event = static_cast<const InputEventControllerTrigger&>(base);
	return m_deviceNum == event.m_deviceNum && m_type == event.m_type;
}

// CONTROLLER BUTTON

InputEventControllerButton::InputEventControllerButton(int deviceNum, int xinputButton) :
	m_deviceNum(deviceNum), m_xinputButton(xinputButton)
{}

void InputEventControllerButton::Update(float dt)
{
#ifdef PLATFORM_WINDOWS
	m_isDown = App::GetController(m_deviceNum).CheckButton(m_xinputButton);
	m_strength = m_isDown ? 1.0f : 0.0f;
#endif
}

bool InputEventControllerButton::EqualsImpl(const InputEvent& base) const
{
	const auto& event = static_cast<const InputEventControllerButton&>(base);
	return m_deviceNum == event.m_deviceNum
		&& m_xinputButton == event.m_xinputButton;
}


