#pragma once

#include <engine.h>

#include "main_menu/main_menu_scene.h"
#include "game/game_scene.h"

class Minigolf : public Application
{
public:
	void Init();
	void Shutdown();
	void Update(float dt);
	void Render();

	void SwitchToGame();
	void OnGameEnd(int score);

	int GetHighscore() const;

private:
	Scene* m_currentScene;
	std::unique_ptr<MainMenuScene> m_mainMenu;
	std::unique_ptr<GameScene> m_gameScene;

	int m_highScore = 0;

	//bool m_paused = false;
	//RID m_pauseTxt;
};

REGISTER_GAME(Minigolf)
