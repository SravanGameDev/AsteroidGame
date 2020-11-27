#include "Background.h"
#include "System.h"
#include "Graphics.h"
#include "ImmediateMode.h"
#include "Random.h"

Background::Background(float width, float height)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	for (int i = 0; i < NUM_STARS; i++)
	{
		stars_[i].x = Random::GetFloat(-halfWidth, halfWidth);
		stars_[i].y = Random::GetFloat(-halfHeight, halfHeight);
		stars_[i].z = 100.0f;
		stars_[i].diffuse = 0xffffffff;
	}
}

void Background::Update(System *systems)
{
}

void Background::Render(Graphics *graphics) const
{
	graphics->ClearFrame(0.0f, 0.0f, 0.0f, 0.0f);

	graphics->GetImmediateMode()->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		&stars_[0],
		NUM_STARS);
}
