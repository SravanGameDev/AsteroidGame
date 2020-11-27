#ifndef ORTHOCAMERA_H_INCLUDED
#define ORTHOCAMERA_H_INCLUDED

#include <DirectXMath.h>

using namespace DirectX;

class Graphics;

class OrthoCamera
{
public:
	OrthoCamera();

	void SetPosition(const XMFLOAT3 &position);
	void SetFrustum(float width,
		float height,
		float nearZ,
		float farZ);

	void SetAsView(Graphics *graphics) const;

private:

	XMFLOAT3 position_;
	XMFLOAT2 widthHeight_;
	XMFLOAT2 nearFarZ_;

};

#endif // ORTHOCAMERA_H_INCLUDED
