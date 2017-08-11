// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};
cbuffer ConstantBuffer : register( b0 )
{
	int radarId;
}
// 8bit texture
Texture2D tx8bit : register( t0 );
SamplerState tx8bit_Sampler : register( s0 );
// Unification coef texture
Texture2D txUnificationCoefs : register( t1 );
SamplerState txUnificationCoefs_Sampler : register( s1 );

float4 PS(PS_INPUT input) : SV_Target
{
	float4 res = float4(0, 0, 0, 0);
	float4 val8bit = tx8bit.Sample(tx8bit_Sampler, input.Texture);
	float4 valUnificationCoef = txUnificationCoefs.Sample(txUnificationCoefs_Sampler, input.Texture);
	res[0] = val8bit[0];
	res[1] = valUnificationCoef[0];
	res[2] = 0;
	res[3] = radarId / 255.0f; //radarId should be in [0, 255]
	return res;
}


//texture coordinates setup for Unification coef texture
//U coord is already fine as it keeps distance (circle triangulation)
//V coord is to be centered, so that distance by V to texels is the same for all pixels with the same U coord
//input.Texture[1] = 0; //actually not needed as does not change result
//float4 valUnificationCoef = txUnificationCoefs.Sample(txUnificationCoefs_Sampler, input.Texture);
////Unification coef is packed into Green and Red channel in case of R8G8B8A8 format
//if (valUnificationCoef[0] == 1) //in this case formulas below give wrong result
//{
//	res[1] = 1;
//	res[2] = 0;
//}
//else
//{
//	res[1] = frac(valUnificationCoef[0]) - frac(valUnificationCoef[0] * 255.0) / 255.0;
//	res[2] = frac(valUnificationCoef[0] * 255.0) - frac(valUnificationCoef[0] * 65025) / 255.0;
//}