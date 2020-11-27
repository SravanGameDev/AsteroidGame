Texture2D fontTexture;
SamplerState fontSampler;

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 Uv : TEXCOORD;
	float4 Colour : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float textureSizeX, textureSizeY;
	fontTexture.GetDimensions(textureSizeX, textureSizeY);
	return input.Colour * fontTexture.Sample(fontSampler, input.Uv / float2(textureSizeX, textureSizeY));
}
