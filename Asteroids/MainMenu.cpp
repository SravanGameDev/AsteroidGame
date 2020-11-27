#include "MainMenu.h"
#include "System.h"
#include "Graphics.h"
#include "Game.h"
#include "FontEngine.h"
#include "Keyboard.h"

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::OnActivate(System *system, StateArgumentMap &args)
{
}

void MainMenu::OnUpdate(System *system)
{
	Keyboard *keyboard = system->GetKeyboard();
	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		GameState::StateArgumentMap args;
		args["Level"].asInt = 1;
		system->SetNextState("LevelStart", args);
	}
}

void MainMenu::OnRender(System *system)
{
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);

	fontEngine->DrawText("ASTEROIDS", 50, 50, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);
	fontEngine->DrawText("Press [Space] to Start", 50, 100, 0xffffffff, FontEngine::FONT_TYPE_SMALL);
}

void MainMenu::OnDeactivate(System *system)
{
}
