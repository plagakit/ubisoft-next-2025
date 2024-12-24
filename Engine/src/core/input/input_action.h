#pragma once

//#include <Winuser.h> // for virtual keycodes
//#include <Xinput.h> // for XINPUT codes

#include "core/input/input_event.h"

#include <string>
#include <vector>
#include <memory>

class InputAction
{
public:
	InputAction(std::string name);
	void AddEvent(std::unique_ptr<InputEvent> event);

private:
	friend class Input;

	std::string m_name;
	std::vector<std::unique_ptr<InputEvent>> m_events;
	bool m_pressed;
	bool m_justPressed;

	void Update();
	bool Pressed() const;
	bool JustPressed() const;
	float Strength() const;
};

