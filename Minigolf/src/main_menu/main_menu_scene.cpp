#include "main_menu_scene.h"

MainMenuScene::MainMenuScene(Application& game) :
	Scene(game)
{
	m_startButtonFont = m_resourceMgr.Load<Font>("HELVETICA_18");
	Font& font = m_resourceMgr.Get<Font>(m_startButtonFont);

	std::unique_ptr<Button> startGame = std::make_unique<Button>(
		m_input,
		Dim2(0.5f, 0.5f, 0.0f, 0.0f),
		Dim2(0.0f, 0.0f, 250.0f, 75.0f),
		Color::DARK_GRAY, Color::BLACK, Color::GRAY, Color::WHITE
	);
	m_startButton = startGame.get();
	startGame->GetAnchor() = Vec2(0.5f, 0.5f);
	startGame->s_Clicked.Connect<MainMenuScene, &MainMenuScene::EmitStartGame>(this);
	startGame->s_Hovered.Connect<MainMenuScene, &MainMenuScene::HoverStartButton>(this);
	startGame->s_Unhovered.Connect<MainMenuScene, &MainMenuScene::UnhoverStartButton>(this);

	std::unique_ptr<Label> startGameLabel = std::make_unique<Label>(
		"Start Game",
		m_startButtonFont,
		Color::WHITE
	);
	startGameLabel->GetPosition() = Dim2(0.5f, 0.5f, -40.0f, -font.GetFontHeight() * 0.5f);
	
	auto byLabel = std::make_unique<Label>(
		"Made by Thomas Plagakis for Ubisoft NEXT 2025",
		m_startButtonFont,
		Color::WHITE
	);
	byLabel->GetPosition() = Dim2(0.5f, 0.4f, -200.0f, -font.GetFontHeight() * 0.5f);

	m_GUI->AddChild(std::move(byLabel));
	startGame->AddChild(std::move(startGameLabel));
	m_GUI->AddChild(std::move(startGame));
}

MainMenuScene::~MainMenuScene()
{
	m_resourceMgr.Unload(m_startButtonFont);
}

void MainMenuScene::Update(float dt)
{
	m_GUI->UpdateGUI(dt);
}

void MainMenuScene::Render()
{
	// Render a nice gradient
	Color start = Color(0.0f, 0.725f, 1.0f);
	Color end = Color(0.5f, 0.0f, 1.0f);
	for (float y = 0.0f; y < APP_VIRTUAL_HEIGHT; y++)
		m_renderer.DrawScreenLine(0.0f, y, APP_VIRTUAL_WIDTH, y,
			Math::Lerp(start, end, 1.0f - y / APP_VIRTUAL_HEIGHT));

	m_GUI->RenderGUI(m_renderer);
}

void MainMenuScene::EmitStartGame()
{
	s_StartedGame.Emit();
}

void MainMenuScene::HoverStartButton()
{
	m_tweenMgr.CreateTween<Vec2>(
		Vec2(250.0f, 75.0f), Vec2(300.0f, 100.0f),
		m_startButton->GetSize().offset, 0.3f,
		EasingMode::CUBIC, EasingType::OUT
	);
}

void MainMenuScene::UnhoverStartButton()
{
	m_tweenMgr.CreateTween<Vec2>(
		Vec2(300.0f, 100.0f), Vec2(250.0f, 75.0f),
		m_startButton->GetSize().offset, 0.3f,
		EasingMode::CUBIC, EasingType::OUT
	);
}
