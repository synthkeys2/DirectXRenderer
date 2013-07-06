TextureCube SkyMap : register(t0);
SamplerState someSampler : register(s0);


struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return SkyMap.Sample(someSampler, input.texCoord);
}