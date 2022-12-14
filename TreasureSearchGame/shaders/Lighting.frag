#version 330                                                                        
                                                                                    
const int MAX_POINT_LIGHTS = 10;                                                     
const int MAX_SPOT_LIGHTS = 10;                                                      
                                                                                    
in vec4 LightSpacePos;                                                              
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
                                                                                    
out vec4 FragColor;                                                                 
                                                                                    
struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                          
    vec3 Direction;                                                                 
};                                                                                  
                                                                                    
struct Attenuation                                                                  
{                                                                                   
    float Constant;                                                                 
    float Linear;                                                                   
    float Exp;                                                                      
};                                                                                  
                                                                                    
struct PointLight                                                                           
{                                                                                           
    BaseLight Base;                                                                  
    vec3 Position;                                                                          
    Attenuation Atten;                                                                      
};                                                                                          
                                                                                            
struct SpotLight                                                                            
{                                                                                           
    PointLight Base;                                                                 
    vec3 Direction;                                                                         
    float Cutoff;                                                                           
};                                                                                          
                                                                                            
uniform int gNumPointLights;                                                                
uniform int gNumSpotLights;                                                                 
uniform DirectionalLight gDirectionalLight;                                                 
uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                                             
uniform sampler2D gSampler;                                                                 
uniform sampler2D gShadowMap;                                                               
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;                                                               
uniform float gAlpha;                                                               
                                                                                            
float CalcShadowFactor(vec4 LightSpacePos)                                                  
{                                                                                           
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;                                  
    vec2 UVCoords;                                                                          
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;                                                  
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;                                                  
    float z = 0.5 * ProjCoords.z + 0.5;                                                     
    float Depth = texture(gShadowMap, UVCoords).x;                                          
    if (Depth < z + 0.00001)                                                                 
        return 0.5;                                                                         
    else                                                                                    
        return 1.0;                                                                         
}                                                                                           
                                                                                            
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal,            
                       float ShadowFactor)                                                  
{                                                                                           
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                            
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (DiffuseFactor > 0) {                                                                
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);    
                                                                                            
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                             
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                                      
        if (SpecularFactor > 0) {                                                           
            SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
            SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;                         
        }                                                                                   
    }                                                                                       
    // ShadowFactor = 1.0f;                
                    
    return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor));                  
}                                                                                           
                                                                                            
vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                                
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);  
}                                                                                                
                                                                                            
vec4 CalcPointLight(PointLight l, vec3 Normal, vec4 LightSpacePos)                   
{                                                                                           
    vec3 LightDirection = WorldPos0 - l.Position;                                           
    float Distance = length(LightDirection);                                                
    LightDirection = normalize(LightDirection);                                             
    // float ShadowFactor = CalcShadowFactor(LightSpacePos);                                   
    
    float ShadowFactor = 1.0f;
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal, ShadowFactor);           
    float AttenuationFactor =  l.Atten.Constant +                                                 
                         l.Atten.Linear * Distance +                                        
                         l.Atten.Exp * Distance * Distance;                                 
    // return Color;
    return Color / AttenuationFactor;                                                             
}                                                                                           
                                                                                            
vec4 CalcSpotLight(SpotLight l, vec3 Normal, vec4 LightSpacePos)                     
{                                                                                           
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);                             
    float SpotFactor = dot(LightToPixel, l.Direction);                                      
                                                                                            
    if (SpotFactor > l.Cutoff) {                                                            
        vec4 Color = CalcPointLight(l.Base, Normal, LightSpacePos);                         
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   
    }                                                                                       
    else {                                                                                  
        return vec4(0,0,0,0);                                                               
    }                                                                                       
}                                                                                           
                                                                                            
void main()                                                                                 
{           
	// Surface normal
	// vec3 N = normalize(Normal0);
	// // Vector from surface to light
	// vec3 L = normalize(-gSpotLights[0].Direction);
	// // Vector from surface to camera
	// vec3 V = normalize(gEyeWorldPos - WorldPos0);
	// // Reflection of -L about N
	// vec3 R = normalize(reflect(-L, N));
    // 
	// // Compute phong reflection
	// vec3 Phong = gSpotLights[0].Base.Base.Color;
	// float NdotL = dot(N, L);
	// if (NdotL > 0)
	// {
	// 	vec3 Diffuse = vec3(0.8f, 0.8f, 0.8f) * NdotL;
	// 	vec3 Specular = vec3(0.8f, 0.8f, 0.8f) * pow(max(0.0, dot(R, V)), 323.0f);
	// 	Phong += Diffuse + Specular;
	// }
    // 
	// // FragColor = texture(tex0, texCoord);
	// FragColor = texture(gSampler, TexCoord0) * vec4(Phong, 1.0f);

    // -------------------------------
    vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight;
    // TotalLight = CalcDirectionalLight(Normal);  
    
                                                                                            
    // for (int i = 0 ; i < gNumPointLights ; i++) {                                           
    //     TotalLight += CalcPointLight(gPointLights[i], Normal, LightSpacePos);               
    // }                                                                                       
    //                                                                                         
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                            
        TotalLight += CalcSpotLight(gSpotLights[i], Normal, LightSpacePos);                 
    }                                                                                       
                                                                                            
    vec4 SampledColor = texture2D(gSampler, TexCoord0.xy);                                  
    //if(SampledColor.a < 0.4) {
    //    discard;
    //}

    FragColor = SampledColor * TotalLight;  
    FragColor.a = gAlpha;
}
