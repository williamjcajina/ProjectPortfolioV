// A constant buffer that stores the three basic column-major matrices for composing geometry.



cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
 
  
};
cbuffer Animation: register(b1)
{

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
    float3 worldPos : TEXCOORD0;
    float3 localPos : TEXCOORD1;
  
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
   
    PixelShaderInput output;
    float4 pos = float4(input.pos, 1.0f);
    float4 blend;


	 //pos = mul(pos, jointsCurrent[input.blendIndecis.x]);
	 
    blend = mul(pos, jointsCurrent[input.blendIndecis.x]) * input.weights.x;
    blend += mul(pos, jointsCurrent[input.blendIndecis.y]) * input.weights.y;
    blend += mul(pos, jointsCurrent[input.blendIndecis.z]) * input.weights.z;
    blend += mul(pos, jointsCurrent[input.blendIndecis.w]) * input.weights.w;

	
    pos = blend;
    
    output.localPos = pos;
    output.worldPos = mul(pos, model);


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

