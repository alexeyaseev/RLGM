#define MAX_NUM_RADARS 10 //maximum number of radars

// Pixel shader input structure
struct PS_INPUT
{
	float4 Position   : SV_POSITION;
	float2 Texture    : TEXCOORD0;
};

cbuffer ConstantBuffer : register( b0 )
{
	int numRadars;			 //number of radar
	int unification8bitRule; //unification rule: 0 - max, 1 - linear combination
}

//8bit textures
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D texture4 : register(t4);
Texture2D texture5 : register(t5);
Texture2D texture6 : register(t6);
Texture2D texture7 : register(t7);
Texture2D texture8 : register(t8);
Texture2D texture9 : register(t9);
SamplerState txSampler : register(s0);

float4 Integrate(float4 vals8bit[MAX_NUM_RADARS], int num_radars, int unification_8bit_rule)
{
	float4 res = float4(0, 0, 0, 0);

	//maximum rule
	if (unification_8bit_rule == 0)
	{
		int index = -1;
		float max = -1;
		for (int k = 0; k < num_radars; k++)
		{
			if (vals8bit[k][3] > 0) //if 8bit sample exists
			{
				float max_candidate = vals8bit[k][0] * vals8bit[k][1]; //8bit ampl multiplied by unification coef
				if (max_candidate > max)
				{
					index = k;
					max = max_candidate;
				}
			}
		}		
		if (index != -1) //check if there were any 8bit sample
			res = vals8bit[index];
	}
	
	//linear combination
	if (unification_8bit_rule == 1)
	{
		float ampl = 0;
		float coefs = 0;
		float radar_id = -1; //output radar id (choosen as any one from existing)
		for (int k = 0; k < num_radars; k++)
		{	
			if (vals8bit[k][3] > 0) //if 8bit sample exists
			{
				radar_id = vals8bit[k][3];				  //save radar id
				ampl  += vals8bit[k][1] * vals8bit[k][0]; //8bit ampl multiplied by unification coef
				coefs += vals8bit[k][1];				  //unification coef
			}
		}		
		if (radar_id != 1) //check if there were any 8bit sample
		{
			res[0] = ampl / coefs;
			res[3] = radar_id;
		}
	}

	return res;
}

float4 PS(PS_INPUT input) : SV_TARGET 
{
  float4 vals8bit[MAX_NUM_RADARS];
  vals8bit[0] = texture0.Sample(txSampler, input.Texture);
  vals8bit[1] = texture1.Sample(txSampler, input.Texture);
  vals8bit[2] = texture2.Sample(txSampler, input.Texture);
  vals8bit[3] = texture3.Sample(txSampler, input.Texture);
  vals8bit[4] = texture4.Sample(txSampler, input.Texture);
  vals8bit[5] = texture5.Sample(txSampler, input.Texture);
  vals8bit[6] = texture6.Sample(txSampler, input.Texture);
  vals8bit[7] = texture7.Sample(txSampler, input.Texture);
  vals8bit[8] = texture8.Sample(txSampler, input.Texture);
  vals8bit[9] = texture9.Sample(txSampler, input.Texture);

  return Integrate(vals8bit, numRadars, unification8bitRule);
}


//(vals8bit[k][1] + vals8bit[k][2] / 255.0) //if unification coef is packed into R8G8B8A8 format