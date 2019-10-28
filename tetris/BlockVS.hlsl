cbuffer Transform
{
	float4x4 world;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return mul(pos, world);
}