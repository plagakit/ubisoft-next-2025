#pragma once

#include "core/input/input_action.h"
#include "core/resource/resource.h"
#include "math/vector/vector2.h"
#include "core/debug/assert.h"

#include <string>
#include <unordered_map>

class Input
{
public:
	void InitDefaultActions();

	void CreateAction(const std::string& name);

	template<typename InputEventType, typename... Args>
	void AddEvent(const std::string& name, Args... args);

	void Update(float dt);
	bool IsPressed(const std::string& action) const;
	bool IsJustPressed(const std::string& action) const;
	float GetStrength(const std::string& action) const;
	float GetAxis(const std::string& negAction, const std::string& posAction) const;
	Vec2 GetMousePos() const;


private:
	std::vector<std::unique_ptr<InputEvent>> m_events;
	std::unordered_map<std::string, std::unique_ptr<InputAction>> m_actionMap;

	Vec2 m_curMousePos;
};

template<typename InputEventType, typename ...Args>
inline void Input::AddEvent(const std::string& name, Args ...args)
{
	static_assert(std::is_base_of<InputEvent, InputEventType>::value, "Must be adding an InputEvent in AddEvent!");
	ASSERT_ERROR(m_actionMap.find(name) != m_actionMap.end(), "Action %s doesn't exist! (Input::AddEvent)", name.c_str());

	auto& action = m_actionMap[name];

	// Perfect-forward the args to make the input event
	std::unique_ptr<InputEvent> event = std::make_unique<InputEventType>(std::forward<Args>(args)...);

	// See if there's any other matching events
	// If so, reuse it (event will go out of scope and delete itself)
	for (auto& e : m_events)
	{
		if (e->Equals(*event.get()))
		{
			action->AddEvent(e.get());
			return;
		}
	}
		
	InputEvent* ptr = event.get();
	action->AddEvent(ptr);
	m_events.push_back(std::move(event));
}
