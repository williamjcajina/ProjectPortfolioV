


struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 uv : UV;
	float3 normal : NORMAL;

};


float4 main(PixelShaderInput input) : SV_TARGET
{







   
	return float4(0.6*input.uv,1);






}
