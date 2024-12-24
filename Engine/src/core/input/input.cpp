#include "pch.h"
#include "input.h"

#include "core/logger/logger.h"
#include <App/app.h>


void Input::InitDefaultActions()
{
	InputAction& left = CreateAction("left");
	left.AddEvent(std::make_unique<InputEventVirtual>(VK_LEFT));
	left.AddEvent(std::make_unique<InputEventVirtual>('A'));
	left.AddEvent(std::make_unique<InputEventControllerTrigger>(0, InputEventControllerTrigger::THUMB_L_LEFT));

	InputAction& right = CreateAction("right");
	right.AddEvent(std::make_unique<InputEventVirtual>(VK_RIGHT));
	right.AddEvent(std::make_unique<InputEventVirtual>('D'));
	right.AddEvent(std::make_unique<InputEventControllerTrigger>(0, InputEventControllerTrigger::THUMB_L_RIGHT));

	InputAction& up = CreateAction("up");
	up.AddEvent(std::make_unique<InputEventVirtual>(VK_UP));
	up.AddEvent(std::make_unique<InputEventVirtual>('W'));
	up.AddEvent(std::make_unique<InputEventControllerTrigger>(0, InputEventControllerTrigger::THUMB_L_UP));

	InputAction& down = CreateAction("down");
	up.AddEvent(std::make_unique<InputEventVirtual>(VK_UP));
	up.AddEvent(std::make_unique<InputEventVirtual>('S'));
	up.AddEvent(std::make_unique<InputEventControllerTrigger>(0, InputEventControllerTrigger::THUMB_L_DOWN));

	InputAction& mouseLeft = CreateAction("mouse-left");
	mouseLeft.AddEvent(std::make_unique<InputEventVirtual>(VK_LBUTTON));
	mouseLeft.AddEvent(std::make_unique<InputEventControllerButton>(0, XINPUT_GAMEPAD_LEFT_SHOULDER));

	InputAction& mouseRight = CreateAction("mouse-right");
	mouseRight.AddEvent(std::make_unique<InputEventVirtual>(VK_RBUTTON));
	mouseLeft.AddEvent(std::make_unique<InputEventControllerButton>(0, XINPUT_GAMEPAD_RIGHT_SHOULDER));
}

InputAction& Input::CreateAction(std::string name)
{
	auto result = m_actions.insert({ name, InputAction(name) });
	return result.first->second;
}

InputAction* Input::GetAction(std::string action)
{
	auto pair = m_actions.find(action);
	if (pair == m_actions.end())
		return nullptr;
	return &pair->second;
}

void Input::Update()
{
	for (auto& pair : m_actions)
		pair.second.Update();
}

bool Input::IsPressed(std::string action) const
{
	auto pair = m_actions.find(action);
	if (pair == m_actions.end())
	{
		Logger::Error("Action %s not found in IsPressed!", action.c_str());
		return false;
	}
	return m_actions.at(action).Pressed();
}

bool Input::IsJustPressed(std::string action) const
{
	auto pair = m_actions.find(action);
	if (pair == m_actions.end())
	{
		Logger::Error("Action %s not found in IsJustPressed!", action.c_str());
		return false;
	}
	return pair->second.JustPressed();
}

float Input::GetStrength(std::string action) const
{
	auto pair = m_actions.find(action);
	if (pair == m_actions.end())
	{
		Logger::Error("Action %s not found in GetStrength!", action.c_str());
		return 0.0f;
	}
	return pair->second.Strength();
}

float Input::GetAxis(std::string negAction, std::string posAction) const
{
	auto neg = m_actions.find(negAction);
	if (neg == m_actions.end())
	{
		Logger::Error("Action %s not found in GetAxis!", negAction.c_str());
		return 0.0f;
	}

	auto pos = m_actions.find(posAction);
	if (pos == m_actions.end())
	{
		Logger::Error("Action %s not found in GetAxis!", posAction.c_str());
		return 0.0f;
	}

	return -(neg->second.Strength()) + pos->second.Strength();
}

Vec2 Input::GetMousePos() const
{
	Vec2 p;
	App::GetMousePos(p.x, p.y);
	return p;
}
