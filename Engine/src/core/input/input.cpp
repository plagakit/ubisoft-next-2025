#include "pch.h"
#include "input.h"

#include "core/debug/assert.h"
#include <App/app.h>

std::unordered_map<std::string, InputAction> Input::m_actions = {};

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
	down.AddEvent(std::make_unique<InputEventVirtual>(VK_DOWN));
	down.AddEvent(std::make_unique<InputEventVirtual>('S'));
	down.AddEvent(std::make_unique<InputEventControllerTrigger>(0, InputEventControllerTrigger::THUMB_L_DOWN));

	InputAction& q = CreateAction("Q");
	q.AddEvent(std::make_unique<InputEventVirtual>('Q'));
	InputAction& e = CreateAction("E");
	e.AddEvent(std::make_unique<InputEventVirtual>('E'));

	InputAction& I = CreateAction("I");
	I.AddEvent(std::make_unique<InputEventVirtual>('I'));
	InputAction& J = CreateAction("J");
	J.AddEvent(std::make_unique<InputEventVirtual>('J'));
	InputAction& K = CreateAction("K");
	K.AddEvent(std::make_unique<InputEventVirtual>('K'));
	InputAction& L = CreateAction("L");
	L.AddEvent(std::make_unique<InputEventVirtual>('L'));

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

bool Input::IsPressed(std::string action) 
{
	auto pair = m_actions.find(action);
	ASSERT_ERROR(pair != m_actions.end(), "Action %s not found!", action.c_str());
	return m_actions.at(action).Pressed();
}

bool Input::IsJustPressed(std::string action)
{
	auto pair = m_actions.find(action);
	ASSERT_ERROR(pair != m_actions.end(), "Action %s not found!", action.c_str());
	return pair->second.JustPressed();
}

float Input::GetStrength(std::string action) 
{
	auto pair = m_actions.find(action);
	ASSERT_ERROR(pair != m_actions.end(), "Action %s not found!", action.c_str());
	return pair->second.Strength();
}

float Input::GetAxis(std::string negAction, std::string posAction) 
{
	auto neg = m_actions.find(negAction);
	auto pos = m_actions.find(posAction);

	ASSERT_ERROR(neg != m_actions.end(), "Action %s not found!", negAction.c_str());
	ASSERT_ERROR(pos != m_actions.end(), "Action %s not found!", posAction.c_str());

	return -(neg->second.Strength()) + pos->second.Strength();
}

Vec2 Input::GetMousePos() 
{
	Vec2 p;
	App::GetMousePos(p.x, p.y);
	return p;
}
