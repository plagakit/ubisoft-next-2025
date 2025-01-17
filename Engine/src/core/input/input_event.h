#pragma once

class InputEvent
{
public:
	virtual void Update(float dt) = 0;
	virtual bool Equals(const InputEvent& event);

	bool IsDown() const;
	float Strength() const;

protected:
	virtual bool EqualsImpl(const InputEvent& event) const = 0;

	bool m_isDown;
	float m_strength;
};

class InputEventVirtual : public InputEvent
{
public:
	InputEventVirtual(unsigned char virtualKey);
	void Update(float dt) override;

protected:
	bool EqualsImpl(const InputEvent& event) const override;

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

	InputEventControllerTrigger(int deviceNum, Type type);
	void Update(float dt) override;

protected:
	bool EqualsImpl(const InputEvent& event) const override;

private:
	int m_deviceNum;
	Type m_type;
};

class InputEventControllerButton : public InputEvent
{
public:
	InputEventControllerButton(int deviceNum, int xinputButton);
	void Update(float dt) override;

protected:
	bool EqualsImpl(const InputEvent& event) const override;

private:
	int m_deviceNum;
	int m_xinputButton;
};