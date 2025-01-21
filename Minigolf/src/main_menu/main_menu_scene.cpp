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
		Color(0.8f, 0.4f, 1.0f), Color::BLACK, Color(0.9f, 0.7f, 1.0f), Color::WHITE
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

	auto hsLabel = std::make_unique<Label>(
		"High Score: ",
		m_startButtonFont,
		Color::WHITE
	);
	hsLabel->GetPosition() = Dim2(0.5f, 0.3f, -100.0f, 0.0f);
	m_highscoreLabel = hsLabel.get();

	auto cprBtn = std::make_unique<Button>(
		m_input,
		Dim2(0.5f, 0.2f, 0.0f, 0.0f),
		Dim2(0.0f, 0.0f, 300.0f, 75.0f),
		Color(0.8f, 0.4f, 1.0f), Color::BLACK, Color(0.9f, 0.7f, 1.0f), Color::WHITE
	);
	m_toggleMusic = cprBtn.get();
	m_toggleMusic->GetAnchor() = Vec2(0.5f, 0.5f);
	m_toggleMusic->s_Clicked.Connect<MainMenuScene, &MainMenuScene::OnClickCopyrightMusicButton>(this);

	auto cprBtnLbl = std::make_unique<Label>(
		"Toggle Copyrighted Music: On",
		m_startButtonFont,
		Color::WHITE
	);
	m_toggleMusicText = cprBtnLbl.get();
	m_toggleMusicText->GetPosition() = Dim2(0.5f, 0.5f, -130.0f, -font.GetFontHeight() * 0.5f);
	m_toggleOn = true;

	m_toggleMusic->AddChild(std::move(cprBtnLbl));
	m_GUI->AddChild(std::move(cprBtn));
	m_GUI->AddChild(std::move(hsLabel));
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

bool MainMenuScene::IsCopyrightMusicOn() const
{
	return m_toggleOn;
}

void MainMenuScene::UpdateHighscore(int highscore)
{
	m_highscoreLabel->GetText() = "High Score: Round " + std::to_string(highscore);
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

void MainMenuScene::OnClickCopyrightMusicButton()
{
	m_toggleOn = !m_toggleOn;
	m_toggleMusicText->GetText() = std::string("Toggle Copyrighted Music: ") + (m_toggleOn ? "On" : "Off");
}
