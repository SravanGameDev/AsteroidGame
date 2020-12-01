#include "Score.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "System.h"
#include "Game.h"
#include "Keyboard.h"

Score::Score() :playerScore_(0)
{
	playerScore_ = 0;
}

Score::~Score()
{

}

void Score::Render(Graphics* graphics) const
{
	FontEngine *fontEngine = graphics->GetFontEngine();

	char playerScoreText[256];
	sprintf_s(playerScoreText, "Score: %d", playerScore_);
	fontEngine->DrawTextA(playerScoreText, 10, 10, 0x0000ff, FontEngine::FONT_TYPE_SMALL);
}

void Score::Update(System* system)
{

}
