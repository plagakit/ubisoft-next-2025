#pragma once

#include <engine.h>

class MainMenuScene : public Scene
{
public:
	MainMenuScene(Application& game);
	~MainMenuScene() override;
	void Update(float dt) override;
	void Render() override;

	Signal<> s_StartedGame;

private:
	void EmitStartGame();
	void HoverStartButton();
	void UnhoverStartButton();

	Button* m_startButton;
	RID m_startButtonFont;

};