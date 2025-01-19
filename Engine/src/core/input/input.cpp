#include "pch.h"
#include "input.h"

#include "core/debug/assert.h"
#ifdef PLATFORM_WINDOWS
#include <App/app.h>
#endif


void Input::InitDefaultActions()
{
	CreateAction("left");
	AddEvent<InputEventVirtual>("left", VK_LEFT);
	AddEvent<InputEventVirtual>("left", 'A');
	AddEvent< InputEventControllerTrigger>("left", 0, InputEventControllerTrigger::THUMB_L_LEFT);

	CreateAction("right");
	AddEvent<InputEventVirtual>("right", VK_RIGHT);
	AddEvent<InputEventVirtual>("right", 'D');
	AddEvent<InputEventControllerTrigger>("right", 0, InputEventControllerTrigger::THUMB_L_RIGHT);

	CreateAction("up");
	AddEvent<InputEventVirtual>("up", VK_UP);
	AddEvent<InputEventVirtual>("up", 'W');
	AddEvent<InputEventControllerTrigger>("up", 0, InputEventControllerTrigger::THUMB_L_UP);

	CreateAction("down");
	AddEvent<InputEventVirtual>("down", VK_DOWN);
	AddEvent<InputEventVirtual>("down", 'S');
	AddEvent<InputEventControllerTrigger>("down", 0, InputEventControllerTrigger::THUMB_L_DOWN);

	CreateAction("space");
	AddEvent<InputEventVirtual>("space", VK_SPACE);
	AddEvent<InputEventControllerButton>("space", 0, XINPUT_GAMEPAD_A);

	CreateAction("Q");
	AddEvent<InputEventVirtual>("Q", 'Q');

	CreateAction("E");
	AddEvent<InputEventVirtual>("E", 'E');

	CreateAction("I");
	AddEvent<InputEventVirtual>("I", 'I');

	CreateAction("J");
	AddEvent<InputEventVirtual>("J", 'J');

	CreateAction("K");
	AddEvent<InputEventVirtual>("K", 'K');

	CreateAction("L");
	AddEvent<InputEventVirtual>("L", 'L');

	CreateAction("mouse-left");
	AddEvent<InputEventVirtual>("mouse-left", VK_LBUTTON);
	AddEvent<InputEventControllerButton>("mouse-left", 0, XINPUT_GAMEPAD_LEFT_SHOULDER);

	CreateAction("mouse-right");
	AddEvent<InputEventVirtual>("mouse-right", VK_RBUTTON);
	AddEvent<InputEventControllerButton>("mouse-right", 0, XINPUT_GAMEPAD_RIGHT_SHOULDER);

	// Old system of adding inputs before i rehauled Input

	/*InputAction& left = CreateAction("left");
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
	mouseLeft.AddEvent(std::make_unique<InputEventControllerButton>(0, XINPUT_GAMEPAD_RIGHT_SHOULDER));*/
}

void Input::CreateAction(const std::string& name)
{
	m_actionMap.insert({ name, std::make_unique<InputAction>(name) });
}

//InputAction* Input::GetAction(std::string action)
//{
//	auto pair = m_actions.find(action);
//	if (pair == m_actions.end())
//		return nullptr;
//	return &pair->second;
//}

void Input::Update(float dt)
{
#ifdef PLATFORM_WINDOWS
	App::GetMousePos(m_curMousePos.x, m_curMousePos.y);
#endif

	for (auto& event : m_events)
		event->Update(dt);

	for (auto& [name, action] : m_actionMap)
		action->Update();
}

bool Input::IsPressed(const std::string& action) const
{
	auto it = m_actionMap.find(action);
	ASSERT_ERROR(it != m_actionMap.end(), "Action %s not found! (Input::IsPressed)", action.c_str());
	return it->second->Pressed();
}

bool Input::IsJustPressed(const std::string& action) const
{
	auto it = m_actionMap.find(action);
	ASSERT_ERROR(it != m_actionMap.end(), "Action %s not found! (Input::IsJustPressed)", action.c_str());
	return it->second->JustPressed();
}

float Input::GetStrength(const std::string& action) const 
{
	auto it = m_actionMap.find(action);
	ASSERT_ERROR(it != m_actionMap.end(), "Action %s not found! (Input::GetStrength)", action.c_str());
	return it->second->Strength();
}

float Input::GetAxis(const std::string& negAction, const std::string& posAction) const 
{
	auto neg = m_actionMap.find(negAction);
	auto pos = m_actionMap.find(posAction);

	ASSERT_ERROR(neg != m_actionMap.end(), "Negative action %s not found! (Input::GetAxis)", negAction.c_str());
	ASSERT_ERROR(pos != m_actionMap.end(), "Positive action %s not found! (Input::GetAxis)", posAction.c_str());

	return -(neg->second->Strength()) + pos->second->Strength();
}

Vec2 Input::GetMousePos() const
{
	return m_curMousePos;
}
