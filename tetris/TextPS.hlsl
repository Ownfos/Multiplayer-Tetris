#include "TextHeader.hlsli"

Texture2D t;
sampler s;

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return t.Sample(s, input.texcoord);
}