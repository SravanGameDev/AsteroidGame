#include "LevelStart.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "Game.h"

LevelStart::LevelStart() :
	level_(0),
	delay_(0)
{
}

void LevelStart::OnActivate(System *system, StateArgumentMap &args)
{
	level_ = args["Level"].asInt;
	delay_ = 120;
}

void LevelStart::OnUpdate(System *system)
{
	if (--delay_ == 0)
	{
		GameState::StateArgumentMap args;
		args["Level"].asInt = level_;
		system->SetNextState("PlayingState", args);
	}
}

void LevelStart::OnRender(System *system)
{
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);

	char levelStartText[256];
	sprintf(levelStartText, "Level %d", level_);
	int textWidth = fontEngine->CalculateTextWidth(levelStartText, FontEngine::FONT_TYPE_LARGE);
	int textX = (800 - textWidth) / 2;
	int textY = (600 - 48) / 2;
	fontEngine->DrawText(levelStartText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);
}

void LevelStart::OnDeactivate(System *system)
{
}
