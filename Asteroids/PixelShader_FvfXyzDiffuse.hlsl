struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float4 Colour : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return input.Colour;
}
