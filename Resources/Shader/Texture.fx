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

cbuffer TextureCutData : register(b1)
{
    // 0.0f 0.0f 
    float2 TextureCutDataPos;
    // 1 / 8 1 / 8
    float2 TextureCutDataSize;
};

PS_INPUT Texture_VS(VS_INPUT _in)
{
    PS_INPUT output;
    output.Position = _in.Position;

    output.Position = mul(output.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.TexCoord.x = (_in.TexCoord.x * TextureCutDataSize.x) + TextureCutDataPos.x;
    output.TexCoord.y = (_in.TexCoord.y * TextureCutDataSize.y) + TextureCutDataPos.y;

    return output;
}

Texture2D Tex : register(t0);
SamplerState Sample : register(s0);

float4 Texture_PS(PS_INPUT _in) : SV_Target0
{
    float4 color = Tex.Sample(Sample, float2(_in.TexCoord.xy));
    return color;
}



