#include "TextHeader.hlsli"

cbuffer Transform
{
	float4x4 world;
};

VS_OUTPUT main(float4 pos : POSITION, float2 texcoord : TEXCOORD)
{
	VS_OUTPUT output;
	output.pos = mul(pos, world);
	output.texcoord = texcoord;
	return output;
}