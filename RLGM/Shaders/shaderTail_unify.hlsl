#define MAX_NUM_RADARS 8 //maximum number of radars

// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	int numRadars;  //number of radars
	int thr2bit;    //tail thr
}

// unified tail 2bit
Texture2D texture0 : register(t0);

// unified 2bit
Texture2D texture1 : register(t1);

// tail textures
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D texture4 : register(t4);
Texture2D texture5 : register(t5);
Texture2D texture6 : register(t6);
Texture2D texture7 : register(t7);
Texture2D texture8 : register(t8);
Texture2D texture9 : register(t9);
SamplerState txSampler : register(s0);

//maximum rule (most recent tail)
float4 Unify(float4 valsTail[MAX_NUM_RADARS], int num_radars)
{
	float4 res = float4(0, 0, 0, 0);

	int index = -1;
	float max = -1;
	for (int k = 0; k < num_radars; k++)
	{
		if (valsTail[k][3] > 0) //if tail sample exists
		{
			float max_candidate = valsTail[k][0];
			if (max_candidate > max)
			{
				max = max_candidate;
				index = k;
			}
		}
	}
	
	if (index != -1) //check if there were any tail sample
		res = valsTail[index];
		
	return res;
}

float4 PS(PS_INPUT input) : SV_TARGET 
{
  float4 valsTail[MAX_NUM_RADARS];
  valsTail[0] = texture2.Sample(txSampler, input.Texture);
  valsTail[1] = texture3.Sample(txSampler, input.Texture);
  valsTail[2] = texture4.Sample(txSampler, input.Texture);
  valsTail[3] = texture5.Sample(txSampler, input.Texture);
  valsTail[4] = texture6.Sample(txSampler, input.Texture);
  valsTail[5] = texture7.Sample(txSampler, input.Texture);
  valsTail[6] = texture8.Sample(txSampler, input.Texture);
  valsTail[7] = texture9.Sample(txSampler, input.Texture);
  
  float4 valTail2bit = texture0.Sample(txSampler, input.Texture);
  float4 val2bit     = texture1.Sample(txSampler, input.Texture);

  //do not draw tails over 2bit
  int ampl2bit = (val2bit[0] * 255); //multiply by 255 if 2bit is saved as R8_UNORM
  if (ampl2bit > 0) 
	  return float4(0, 0, 0, 0);

  //do not draw tails when unified 2bit level is not enough
  ampl2bit = (valTail2bit[0] * 255); //multiply by 255 if tail is saved as R8G8_UNORM
  if (ampl2bit <= thr2bit)
	  return float4(0, 0, 0, 0);
  
  return Unify(valsTail, numRadars);
}