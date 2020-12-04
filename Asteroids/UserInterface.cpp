#include "UserInterface.h"
#include "Graphics.h"
#include "FontEngine.h"

UserInterface::UserInterface() :playerScore_(0),highScore_(0)
{
}

UserInterface::~UserInterface()
{

}

void UserInterface::Render(Graphics* graphics) const
{
	int yAxis=10;

	FontEngine *fontEngine = graphics->GetFontEngine();

	char PlayerScoreText[256];
	sprintf_s(PlayerScoreText, "Score\n %d", playerScore_);
	fontEngine->DrawTextA(PlayerScoreText, 10, yAxis, 0x0000ff, FontEngine::FONT_TYPE_SMALL);

	char HighScoreText[256];
	sprintf_s(HighScoreText, "High Score\n %d", highScore_);
	fontEngine->DrawTextA(HighScoreText, 700, yAxis, 0x0000ff, FontEngine::FONT_TYPE_SMALL);

	char PlayerLivesText[256];
	sprintf_s(PlayerLivesText, "Lives\n %d", playerScore_);
	fontEngine->DrawTextA(PlayerLivesText, 350, yAxis, 0x0000ff,FontEngine::FONT_TYPE_MEDIUM);


}

void UserInterface::Update(System* system)
{

}

void UserInterface::ResetScore()
{
	playerScore_ = 0;
}

void UserInterface::UpdateScore()
{
	playerScore_++;
}
