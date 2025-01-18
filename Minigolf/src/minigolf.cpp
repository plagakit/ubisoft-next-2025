#include <engine.h>

#include "main_menu/main_menu_scene.h"
#include "game/game_scene.h"

class Minigolf : public Application
{
private:
	Scene* currentScene;
	std::unique_ptr<MainMenuScene> m_mainMenu;
	std::unique_ptr<GameScene> m_gameScene;

	bool m_paused = false;
	RID m_pauseTxt;

public:
	void SwitchToGame()
	{
		m_gameScene = std::make_unique<GameScene>(*this);
		currentScene = m_gameScene.get();
	}

	void Init()
	{
		m_input->InitDefaultActions();
		m_input->CreateAction("pause");
		m_input->AddEvent<InputEventVirtual>("pause", 'P');

		m_paused = false;
		m_pauseTxt = m_resourceMgr->Load<Texture>("res/sprites/pause_screen.png");

		m_mainMenu = std::make_unique<MainMenuScene>(*this);
		m_mainMenu->s_StartedGame.Connect<Minigolf, &Minigolf::SwitchToGame>(this);
		currentScene = m_mainMenu.get();

		SwitchToGame();
	}

	void Shutdown()
	{

	}

	void Update(float dt)
	{
		if (m_input->IsJustPressed("pause"))
			m_paused = !m_paused;


		if (!m_paused)
			currentScene->Update(dt);
	}

	void Render()
	{
		currentScene->Render();

		if (m_paused)
			m_renderer->DrawTexture(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2, m_pauseTxt);
	}
};

REGISTER_GAME(Minigolf)
