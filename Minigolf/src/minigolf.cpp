#include "minigolf.h"

void Minigolf::Init()
{
	m_input->InitDefaultActions();
	m_input->CreateAction("pause");
	m_input->AddEvent<InputEventVirtual>("pause", 'P');

	//m_paused = false;
	//m_pauseTxt = m_resourceMgr->Load<Texture>("res/sprites/pause_screen.png");

	m_mainMenu = std::make_unique<MainMenuScene>(*this);
	m_mainMenu->s_StartedGame.Connect<Minigolf, &Minigolf::SwitchToGame>(this);
	m_currentScene = m_mainMenu.get();

	//SwitchToGame();
}

void Minigolf::Shutdown()
{
	//if (m_gameScene) 
	//	delete m_gameScene;

	//if (m_mainMenu) 
	//	delete m_mainMenu;
}

void Minigolf::Update(float dt)
{
	if (m_currentScene)
		m_currentScene->Update(dt);
}

void Minigolf::Render()
{
	if (m_currentScene)
		m_currentScene->Render();
}

void Minigolf::SwitchToGame()
{
	m_gameScene = std::make_unique<GameScene>(*this, m_mainMenu->IsCopyrightMusicOn());
	m_gameScene->s_EndedGame.Connect<Minigolf, &Minigolf::OnGameEnd>(this);
	m_currentScene = m_gameScene.get();
}

void Minigolf::OnGameEnd(int score)
{
	if (score > m_highScore)
		m_highScore = score;

	m_gameScene.reset();
	m_mainMenu->UpdateHighscore(score);
	m_currentScene = m_mainMenu.get();
}

int Minigolf::GetHighscore() const
{
	return m_highScore;
}
