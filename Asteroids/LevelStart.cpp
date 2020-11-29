
#include "LevelStart.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "Game.h"

LevelStart::LevelStart() :
	level_(0),
	delay_(0),
	score_(0)

{
}

void LevelStart::OnActivate(System *system, StateArgumentMap &args)
{
	level_ = args["Level"].asInt;
	delay_ = 120;
	score_ = args["Score"].asInt;
}

void LevelStart::OnUpdate(System *system)
{
	if (--delay_ == 0)
	{
		GameState::StateArgumentMap args;
		args["Level"].asInt = level_;
		args["Score"].asInt = score_;
		system->SetNextState("PlayingState", args);
	}
}

void LevelStart::OnRender(System *system)
{
	Graphics *graphics = system->GetGraphics();
	FontEngine *fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);

	char levelStartText[256];
	sprintf_s(levelStartText, "Level %d", level_);
	int textWidth = fontEngine->CalculateTextWidth(levelStartText, FontEngine::FONT_TYPE_LARGE);
	int textX = (800 - textWidth) / 2;
	int textY = (600 - 48) / 2;
	fontEngine->DrawText(levelStartText, textX, textY, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);

	char scoreText[256];
	sprintf_s(scoreText, "Score: %d", score_);
	int scoreWidth = fontEngine->CalculateTextWidth(scoreText, FontEngine::FONT_TYPE_SMALL);
	int scoreX = (800 - scoreWidth) / 20;
	int scoreY = (600 - 48) / 20;
	fontEngine->DrawText(scoreText, scoreX, scoreY, 0xff00ffff, FontEngine::FONT_TYPE_SMALL);
}

void LevelStart::OnDeactivate(System *system)
{
}
