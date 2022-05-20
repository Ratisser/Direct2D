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

cbuffer ResultColor : register(b2)
{
    float4 bufferColor;
}

cbuffer AddColor : register(b3)
{
    float4 addColor;
}

cbuffer AddUV : register(b4)
{
    float4 addUV;
}

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
    _in.TexCoord.x += addUV.x;
    _in.TexCoord.y += addUV.y;
    float4 color = (Tex.Sample(Sample, float2(_in.TexCoord.xy)) * bufferColor);
    
    color.r += addColor.r;
    color.g += addColor.g;
    color.b += addColor.b;
    
    if (color.a <= 0.0f)
    {
        clip(-1);
    }
    
    //if (color.a < 0.0f)
    //{
    //    color.a = 1 - color.a;
        
    //    if (1.0f == color.a)
    //    {
    //        clip(-1);
    //    }
    //}
    //else
    //{
    //    if (0.0f == color.a)
    //    {
    //        clip(-1);
    //    }
    //}
        
    //if (color.r < 0.0f)
    //{
    //    color.r = 1 - color.r;
    //}
    
    //if (color.g < 0.0f)
    //{
    //    color.g = 1 - color.g;
    //}
    
    //if (color.b < 0.0f)
    //{
    //    color.b = 1 - color.b;
    //}
    
    return color;
}



