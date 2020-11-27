#include "BootState.h"
#include "GameState.h"
#include "System.h"
#include "Graphics.h"
#include "FontEngine.h"
#include "AssetLoader.h"

BootState::BootState() :
	state_(STATE_BEGIN)
{
}

BootState::~BootState()
{
}

void BootState::OnActivate(System *system, StateArgumentMap &args)
{
	strings_.push_back("LOADING:");
	fakeDelay_ = 10;
}

void BootState::OnUpdate(System *system)
{
	switch (state_)
	{
	case STATE_BEGIN: UpdateBegin(system); break;
	case STATE_LOADING_FONTS: UpdateLoadingFonts(system); break;
	case STATE_LOADING_MODELS: UpdateLoadingModels(system); break;
	case STATE_LOADING_TEXTURES: UpdateLoadingTextures(system); break;
	case STATE_LOADING_SOUNDS: UpdateLoadingSounds(system); break;
	case STATE_END: UpdateEnd(system); break;
	}
}

void BootState::OnRender(System *system)
{
	system->GetGraphics()->ClearFrame(0.0f, 0.0f, 0.0f, 0.0f);

	int currentLineY = 5;
	for (StringList::iterator stringIt = strings_.begin(), end = strings_.end();
		stringIt != end;
		++stringIt)
	{
		currentLineY += system->GetGraphics()->GetFontEngine()->DrawText(*stringIt, 5, currentLineY, 0xffffffff, FontEngine::FONT_TYPE_SMALL);
	}
}

void BootState::OnDeactivate(System *system)
{
}

void BootState::UpdateBegin(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	strings_.push_back("Loading fonts...");
	state_ = STATE_LOADING_FONTS;
	fakeDelay_ = 10;
}

void BootState::UpdateLoadingFonts(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	strings_.push_back("Loading models...");
	state_ = STATE_LOADING_MODELS;
	fakeDelay_ = 10;
}

void BootState::UpdateLoadingModels(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	strings_.push_back("Loading textures...");
	state_ = STATE_LOADING_TEXTURES;
	fakeDelay_ = 10;
}

void BootState::UpdateLoadingTextures(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	strings_.push_back("Loading sounds...");
	state_ = STATE_LOADING_SOUNDS;
	fakeDelay_ = 10;
}

void BootState::UpdateLoadingSounds(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	strings_.push_back("FINISHED!");
	state_ = STATE_END;
	fakeDelay_ = 10;
}

void BootState::UpdateEnd(System *system)
{
	if (--fakeDelay_ > 0)
		return;

	system->SetNextState("MainMenu");
}
