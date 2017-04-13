Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float3 cameraPosition;
    float Tex;
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float pointRadius;
    float4 pointColor;
    float3 pointLightPosition;
    float specularPower;
    float4 specularColor;
    float3 spotPos;
    float spotInnerRatio;
    float3 conedir;
    float coneRatio;
    float4 spotColor;
    float spotOuterRatio;
    float normalMap;
    float2 padding;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
    float3 localPos : TEXCOORD1;
  
};




float4 main(PixelShaderInput input) : SV_TARGET
{
        
   
  
    float4 textureColor;
    float4 color;
        textureColor = shaderTexture.Sample(SampleType, input.uv);
   
    
    //ambient Light
    color = saturate(ambientColor);
   
   
   
   
    
    
    
     //Point light

    
    float pointAttenuation;
    float pointLightRatio;
    float3 pointDir;
    pointDir = normalize(pointLightPosition - input.worldPos);
  
     pointLightRatio = saturate(dot(pointDir, normalize(input.normal)));
    
	
	
    pointAttenuation = 1.0f - saturate(length(spotPos - input.worldPos) / pointRadius);
    color += pointLightRatio * pointColor * pointAttenuation;
	//Directional and specularlight
    float3 lightDir;
    float lightIntensity;
    float3 viewDirection;
    float3 reflection;
    float4 specular;
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    viewDirection = cameraPosition - input.worldPos;
    viewDirection = normalize(viewDirection);

	//Spot Light
	
    float3 spotDir;
    float spotRatio;
    float spotFactor;
    float spotLightRatio;
    float spotAttenuation;
    spotDir = normalize(spotPos - input.worldPos);
    spotRatio = saturate(dot(-spotDir, normalize(conedir)));
	
    spotFactor = (spotRatio > coneRatio) ? 1 : 0;

    spotLightRatio = saturate(dot(spotDir, normalize(input.normal)));

    spotAttenuation = 1.0f - saturate((spotInnerRatio - spotRatio) / (spotInnerRatio - spotOuterRatio));
    color += spotFactor * spotLightRatio * spotColor * spotAttenuation;

	
    lightDir = -lightDirection;
    lightDir = normalize(lightDir);

 
       lightIntensity = saturate(dot(input.normal, lightDir));
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        //color = saturate(color);
      
         reflection = normalize(2 * lightIntensity * normalize(input.normal) - lightDir);

        specular = pow(saturate(dot(reflection, viewDirection)), specularPower);
    }
	



   
    color = color * textureColor;
    color = saturate(color + specular);

    return color;
   

   
}





