// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	int radarId;
}

// tail texture
Texture2D txTail : register(t0);
SamplerState txTail_Sampler : register(s0);

float4 PS(PS_INPUT input) : SV_Target
{
	float4 res = float4(0, 0, 0, 0);
	float4 valTail = txTail.Sample(txTail_Sampler, input.Texture);
	res[0] = valTail[1];		//tail time
	res[3] = radarId / 255.0f;  //radarId should be in [0, 255]
	return res;
}