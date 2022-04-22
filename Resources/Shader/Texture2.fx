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

PS_INPUT Texture2_VS(VS_INPUT _in)
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
SamplerState Sample2 : register(s0);

float4 Texture2_PS(PS_INPUT _in) : SV_Target0
{
    float4 color = Tex.Sample(Sample2, float2(_in.TexCoord.xy));
    if (0.0f == color.a)
    {
        clip(-1);
    }
    color.x = 1.0f - color.x;
    color.y = 1.0f - color.y;
    color.z = 1.0f - color.z;
    return color;
}



