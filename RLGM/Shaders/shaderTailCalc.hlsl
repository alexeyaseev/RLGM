// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	int thr2bit;    //tail thr
}

// 2bit
Texture2D texture0 : register(t0);
SamplerState texture0_Sampler : register(s0);

// tail
Texture2D texture1 : register(t1);
SamplerState texture1_Sampler : register(s1);

float4 PS(PS_INPUT input) : SV_TARGET
{
  float4 res = float4(0, 0, 0, 0);
  float4 val2bit = texture0.Sample(texture0_Sampler, input.Texture);
  float4 valTail = texture1.Sample(texture1_Sampler, input.Texture);
  
  int ampl2bit = val2bit[0] * 255; //multiply by 255 if 2bit is saved as R8_UNORM
  if (ampl2bit > thr2bit)
  {
	  res[0] = val2bit[0]; //save new 2bit value
	  res[1] = 1.0f;       //reset tail time
  }
  else
  {
	  res[0] = valTail[0];				//keep current 2bit value
	  res[1] = valTail[1] - 1 / 255.0f; //decrease tail time by one (1/255.0f if tail is saved as R8G8_UNORM)
  }
  
  return res;
}