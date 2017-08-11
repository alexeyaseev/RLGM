// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

// unified colored 8bit
Texture2D texture0 : register( t0 );
SamplerState texture0_Sampler : register( s0 );

float4 PS(PS_INPUT input) : SV_TARGET 
{
	return texture0.Sample(texture0_Sampler, input.Texture);
}