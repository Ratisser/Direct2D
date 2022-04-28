#include "CbufferHeader.fx"

struct VS_INPUT
{
    float4 Position : POSITION;
    float4 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 TexCoord : TEXCOORD;
};

PS_INPUT TextureNoCut_VS(VS_INPUT _in)
{
    PS_INPUT output;
    output.Position = _in.Position;

    //output.Position = mul(output.Position, World);
    //output.Position = mul(output.Position, View);
    //output.Position = mul(output.Position, Projection);
    output.TexCoord = _in.TexCoord;

    return output;
}

Texture2D Tex : register(t0);
SamplerState Sample2 : register(s0);

float4 TextureNoCut_PS(PS_INPUT _in) : SV_Target0
{
    float4 color = Tex.Sample(Sample2, float2(_in.TexCoord.xy));
    return color;
}



