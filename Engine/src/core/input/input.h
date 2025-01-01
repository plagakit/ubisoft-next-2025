#pragma once

#include "core/input/input_action.h"
#include "math/vector/vector2.h"

#include <unordered_map>

class Input
{
public:
	static void InitDefaultActions();

	static InputAction& CreateAction(std::string name);
	static InputAction* GetAction(std::string action);

	static void Update();
	static bool IsPressed(std::string action);
	static bool IsJustPressed(std::string action);
	static float GetStrength(std::string action);
	static float GetAxis(std::string negAction, std::string posAction);
	static Vec2 GetMousePos();


private:
	static std::unordered_map<std::string, InputAction> m_actions;
};