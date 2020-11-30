#include "Score.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "System.h"
#include "Game.h"
#include "Keyboard.h"

Score::Score() :playerScore_(0)
{

}

Score::~Score()
{

}

void Score::Render(Graphics* graphics) const
{
	FontEngine *fontEngine = graphics->GetFontEngine();
	fontEngine->DrawTextA("Score: ", 400, 200, 0xffffffff, FontEngine::FONT_TYPE_LARGE);
}

void Score::Update(System* system)
{

}
