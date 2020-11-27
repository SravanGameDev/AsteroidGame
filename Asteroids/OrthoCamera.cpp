#include "OrthoCamera.h"
#include "Graphics.h"
#include "ImmediateMode.h"

OrthoCamera::OrthoCamera() :
	position_(0.0f, 0.0f, 0.0f),
	widthHeight_(1.0f, 1.0f),
	nearFarZ_(0.0, 1.0f)
{
}

void OrthoCamera::SetPosition(const XMFLOAT3 &position)
{
	position_ = position;
}

void OrthoCamera::SetFrustum(float width,
	float height,
	float nearZ,
	float farZ)
{
	widthHeight_ = XMFLOAT2(width, height);
	nearFarZ_ = XMFLOAT2(nearZ, farZ);
}

void OrthoCamera::SetAsView(Graphics *graphics) const
{
	XMMATRIX ortho = XMMatrixOrthographicLH(
		widthHeight_.x,
		widthHeight_.y,
		nearFarZ_.x,
		nearFarZ_.y);

	graphics->GetImmediateMode()->SetProjectionMatrix(ortho);
}
