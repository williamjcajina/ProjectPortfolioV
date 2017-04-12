// A constant buffer that stores the three basic column-major matrices for composing geometry.



cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
 
  
};
cbuffer ModelViewProjectionConstantBuffer: register(b1)
{
    matrix jointsInverse[40];
	matrix jointsCurrent[40];

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
 
	float3 ab = input.blendIndecis;
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

