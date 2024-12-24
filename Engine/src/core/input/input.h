#pragma once

#include "core/input/input_action.h"
#include "math/vector/vector2.h"

#include <unordered_map>

class Input
{
public:
	void InitDefaultActions();

	InputAction& CreateAction(std::string name);
	InputAction* GetAction(std::string action);

	void Update();
	bool IsPressed(std::string action) const;
	bool IsJustPressed(std::string action) const;
	float GetStrength(std::string action) const;
	float GetAxis(std::string negAction, std::string posAction) const;
	Vec2 GetMousePos() const;


private:
	std::unordered_map<std::string, InputAction> m_actions;
};