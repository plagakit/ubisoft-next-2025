#pragma once

#include "core/input/input_event.h"

#include <string>
#include <vector>
#include <memory>

class InputAction
{
public:
	InputAction(const std::string& name, float deadzone = 0.5f);
	void AddEvent(InputEvent* event);

	void Update();
	bool Pressed() const;
	bool JustPressed() const;
	float Strength() const;

private:
	std::string m_name;
	std::vector<InputEvent*> m_events; // a list of non-owning observing pointers to the events, won't get invalidated b/c they r stored in map
	float m_deadzone;

	bool m_pressed;
	bool m_justPressed;
	float m_strength;
};

