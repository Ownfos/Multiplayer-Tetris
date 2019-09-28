cbuffer Color
{
	float4 blockColor;
};

float4 main() : SV_TARGET
{
	return blockColor;
}