#include "pch.h"
#include "input_event.h"

#include <App/app.h>

InputEventVirtual::InputEventVirtual(unsigned char virtualKey) :
	m_virtualKey(virtualKey)
{}

bool InputEventVirtual::IsDown() const
{
	return App::IsKeyPressed(m_virtualKey);
}

float InputEventVirtual::Strength() const
{
	return IsDown() ? 1.0f : 0.0f;
}

InputEventControllerTrigger::InputEventControllerTrigger(int deviceNum, Type type, float deadzone) :
	m_deviceNum(deviceNum), m_type(type), m_deadzone(deadzone)
{}

bool InputEventControllerTrigger::IsDown() const
{
	return Strength() >= m_deadzone;
}

float InputEventControllerTrigger::Strength() const
{
	const CController& c = App::GetController(m_deviceNum);
	float lx = c.GetLeftThumbStickX();
	float ly = c.GetLeftThumbStickY();
	float rx = c.GetRightThumbStickX();
	float ry = c.GetRightThumbStickY();

	switch (m_type)
	{
	case THUMB_L_LEFT:	return lx < 0.0f ? -lx : 0.0f;
	case THUMB_L_RIGHT: return lx > 0.0f ? lx : 0.0f;
	case THUMB_L_DOWN:	return ly < 0.0f ? -ly : 0.0f;
	case THUMB_L_UP:	return ly > 0.0f ? ly : 0.0f;

	case THUMB_R_LEFT:	return rx < 0.0f ? -rx : 0.0f;
	case THUMB_R_RIGHT: return rx > 0.0f ? rx : 0.0f;
	case THUMB_R_DOWN:	return ry < 0.0f ? -ry : 0.0f;
	case THUMB_R_UP:	return ry > 0.0f ? ry : 0.0f;

	case TRIGGER_L: return c.GetLeftTrigger();
	case TRIGGER_R: return c.GetRightTrigger();

	default: return 0.0f;
	}
}

InputEventControllerButton::InputEventControllerButton(int deviceNum, int xinputButton) :
	m_deviceNum(deviceNum), m_xinputButton(xinputButton)
{}

bool InputEventControllerButton::IsDown() const
{
	return App::GetController(m_deviceNum).CheckButton(m_xinputButton);
}

float InputEventControllerButton::Strength() const
{
	return IsDown() ? 1.0f : 0.0f;
}