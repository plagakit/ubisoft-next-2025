#include "pch.h"
#include "input_action.h"

#include "math/math_utils.h"
#include <App/app.h>

InputAction::InputAction(std::string name) :
	m_name(std::move(name)), m_pressed(false), m_justPressed(false)
{}

void InputAction::AddEvent(std::unique_ptr<InputEvent> event)
{
	m_events.push_back(std::move(event));
}

void InputAction::Update()
{
	bool wasPressed = m_pressed;

	m_pressed = false;
	for (const auto& event : m_events)
	{
		if (event->IsDown())
		{
			m_pressed = true;
			break;
		}
	}
	
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
	float max = 0.0f;
	for (const auto& event : m_events)
	{
		float strength = event->Strength();
		if (strength > max)
			max = strength;
	}
	return max;
}