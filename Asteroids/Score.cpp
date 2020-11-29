#include "Score.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "System.h"
#include "Game.h"
#include "Keyboard.h"

Score::Score() :score_(0), highScore_(0) {}

Score::~Score(){}


void Score::OnActivate(System* system, StateArgumentMap& args)
{
	score_ = args["Score"].asInt;

}

void Score::OnUpdate(System* system)
{
	
}

void Score::OnRender(System* system)
{
	Graphics* graphics = system->GetGraphics();
	FontEngine* fontEngine = graphics->GetFontEngine();

	system->GetGame()->RenderBackgroundOnly(graphics);
	

	fontEngine->DrawText("Score", 50, 50, 0xff00ffff, FontEngine::FONT_TYPE_LARGE);
}

void Score::OnDeactivate(System* system)
{
}