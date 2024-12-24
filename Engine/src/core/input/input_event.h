#pragma once

class InputEvent
{
public:
	virtual bool IsDown() const = 0;
	virtual float Strength() const = 0;
};

class InputEventVirtual : public InputEvent
{
public:
	InputEventVirtual(unsigned char virtualKey);
	bool IsDown() const override;
	float Strength() const override;

private:
	unsigned char m_virtualKey;
};

class InputEventControllerTrigger : public InputEvent
{
public:
	enum Type {
		THUMB_L_LEFT,
		THUMB_L_RIGHT,
		THUMB_L_UP,
		THUMB_L_DOWN,
		THUMB_R_LEFT,
		THUMB_R_RIGHT,
		THUMB_R_UP,
		THUMB_R_DOWN,
		TRIGGER_R,
		TRIGGER_L
	};

	InputEventControllerTrigger(int deviceNum, Type type, float deadzone = 0.5f);
	bool IsDown() const override;
	float Strength() const override;

private:
	int m_deviceNum;
	Type m_type;
	float m_deadzone;
};

class InputEventControllerButton : public InputEvent
{
public:
	InputEventControllerButton(int deviceNum, int xinputButton);
	bool IsDown() const override;
	float Strength() const override;

private:
	int m_deviceNum;
	int m_xinputButton;
};