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

	bool IsCopyrightMusicOn() const;
	void UpdateHighscore(int highscore);

private:
	void EmitStartGame();
	void HoverStartButton();
	void UnhoverStartButton();

	Button* m_startButton;
	Label* m_highscoreLabel;
	RID m_startButtonFont;

	Button* m_toggleMusic;
	Label* m_toggleMusicText;
	bool m_toggleOn = false;
	void OnClickCopyrightMusicButton();

};