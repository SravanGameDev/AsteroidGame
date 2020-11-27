cbuffer ModelViewProjection
{
	matrix ModelMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VS_INPUT
{
	float2 Xy : POSITION;
	float2 Uv : TEXCOORD;
	uint4 Diffuse : COLOR;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD;
	float4 Colour : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	float4 pos = mul(float4(input.Xy, 0.0f, 1.0f), ModelMatrix);
	pos = mul(pos, ViewMatrix);
	pos = mul(pos, ProjectionMatrix);

	VS_OUTPUT ret;
	ret.Position = pos;
	ret.Uv = input.Uv;
	ret.Colour = input.Diffuse / 255.0f;

	return ret;
}
