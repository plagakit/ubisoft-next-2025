#include "pch.h"
#include "input_action.h"

#include "core/debug/assert.h"
#include "math/math_utils.h"
#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#endif

InputAction::InputAction(const std::string& name, float deadzone) :
	m_name(name),
	m_deadzone(deadzone),
	m_pressed(false),
	m_justPressed(false),
	m_strength(0.0f)
{}

void InputAction::AddEvent(InputEvent* event)
{
	ASSERT_ERROR(event, "Trying to add a null InputEvent to InputAction '%s'!", m_name.c_str());
	m_events.push_back(event);
}

void InputAction::Update()
{
	//m_pressed = false;
	//for (const auto& event : m_events)
	//{
	//	if (event->IsDown())
	//	{
	//		m_pressed = true;
	//		break;
	//	}
	//}
	
	bool wasPressed = m_pressed;

	m_strength = 0.0f;
	for (const auto& event : m_events)
	{
		float strength = event->Strength();
		if (strength > m_strength)
			m_strength = strength;
	}

	m_pressed = m_strength >= m_deadzone;
	m_justPressed = !wasPressed && m_pressed;
}

bool InputAction::Pressed() const
{
	return m_pressed;
}

bool InputAction::JustPressed() const
{
	return m_justPressed;
}

float InputAction::Strength() const
{
	return m_strength;
}