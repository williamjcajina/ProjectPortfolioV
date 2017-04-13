// A constant buffer that stores the three basic column-major matrices for composing geometry.



cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
 
  
};
cbuffer Animation: register(b1)
{
    matrix jointsInverse[60];
	matrix jointsCurrent[60];

}
// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
    float3 pos : POSITION;
    float3 uv : UV;
    float3 normal : NORMAL;
    float4 blendIndecis : TEXCOORD;
    float4 weights : BINORMAL;

   
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 uv : UV;
    float3 normal : NORMAL;
 
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
   
    PixelShaderInput output;
    float4 pos = float4(input.pos, 1.0f);
	float4 b1 = pos;
	float4 b2 = pos;
	float4 b3 = pos;
	float4 b4 = pos;

	 pos = mul(pos, jointsInverse[input.blendIndecis.x]);
	 pos = mul(pos, jointsCurrent[input.blendIndecis.x]);
	

	/*b1 = mul(b1, jointsInverse[input.blendIndecis.x]);
	b1 = mul(b1, jointsCurrent[input.blendIndecis.x]);
	b1 *= input.weights.x;


	b2 = mul(b2, jointsInverse[input.blendIndecis.y]);
	b2 = mul(b2, jointsCurrent[input.blendIndecis.y]);
	b2 *= input.weights.y;

	b3 = mul(b3, jointsInverse[input.blendIndecis.z]);
	b3 = mul(b3, jointsCurrent[input.blendIndecis.z]);
	b3 *= input.weights.z;

	b4 = mul(b4, jointsInverse[input.blendIndecis.w]);
	b4 = mul(b4, jointsCurrent[input.blendIndecis.w]);
	b4 *= input.weights.w;
	pos = b1+b2+b3 + b4;*/


    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
	
    output.pos = pos;
    output.uv = input.uv;
    output.normal = input.normal;

  
    output.normal = mul(input.normal, (float3x3) model);
    output.normal = normalize(output.normal);
 
  
    

   
  

    return output;
}

