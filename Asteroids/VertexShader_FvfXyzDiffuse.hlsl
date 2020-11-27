cbuffer ModelViewProjection
{
	matrix ModelMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VS_INPUT
{
	float3 Xyz : POSITION;
	uint4 Diffuse : COLOR;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float4 Colour : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	float4 pos = mul(float4(input.Xyz, 1.0f), ModelMatrix);
	pos = mul(pos, ViewMatrix);
	pos = mul(pos, ProjectionMatrix);

	VS_OUTPUT ret;
	ret.Position = pos;
	ret.Colour = input.Diffuse / 255.0f;

	return ret;
}
