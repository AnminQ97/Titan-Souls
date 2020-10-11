#include "Global.fx"

//VertexOutput
struct VertexOutput //픽셀쉐이더로 넘어가는 코드
{
    float4 Position : SV_POSITION0;
    float3 wPosition : POSITION1;//world 좌표 저장 
    float2 Uv : UV0;
};


//Vertex Shader
VertexOutput VS(VertexTexture input)
{
    VertexOutput output;
    //output.Position = input.Position;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

//0 Diffuse
float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(Sampler, input.Uv);
}

//1Mono
float4 PS_Mono(VertexOutput input) : SV_TARGET0
{
	float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
	float3 gray = (diffuse.r + diffuse.g + diffuse.b) / 3.0f;

	return float4(gray, diffuse.a);
}

//2. Intensity
float4 PS_Intensity(VertexOutput input) : SV_TARGET0
{
	return DiffuseMap.Sample(Sampler,input.Uv) * 3.0f;
}

//3. Blue
float4 PS_Blue(VertexOutput input) : SV_TARGET0
{
	float4 diffuse = DiffuseMap.Sample(Sampler,input.Uv);
	diffuse.b *= 3.0f;
	diffuse.rg -= 0.2f;
	return diffuse;
}

//4.Bloom
float BaseSaturation = 2.0f;
float BaseIntensity = 2.0f;
float BloomSaturation = 1.0f;
float BloomIntensity = 2.0f;

float4 AdjustSaturation(float4 color, float saturation)
{
	float cDotw = dot(color, float4(1, 1, 1, 1)); //내적
	return lerp(cDotw, color, saturation);
}

float4 PS_Bloom(VertexOutput input) : SV_TARGET0
{
	float4 diffuse = DiffuseMap.Sample(Sampler,input.Uv);
	float alpha = diffuse.a;
	float4 bloom = DiffuseMap.Sample(Sampler, input.Uv);
	
	diffuse = AdjustSaturation(diffuse, BaseSaturation) * BaseIntensity;
	bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;

	diffuse *= (1 - saturate(bloom)); //clamp(bloom, 0,  1)

	return diffuse + bloom;
}

//5.ColorOverlay 
float3 HSVtoRGB(float3 color)
{
	float4 k = float4(1.0f, 0.66f, 0.33f, 3.0f);
	//HSV를 RGB로 바꾸는 식
	float3 p = abs(frac(color.rrr + k.rgb) * 6.0f - k.rrr);
	return color.b * lerp(k.rrr, saturate(p - k.rrr),color.g);

}

float4 PS_Overlay(VertexOutput input) : SV_TARGET0
{
	float3 uvColor = float3(input.Uv.x, 1.0f, 1.0f);
	float alpha = DiffuseMap.Sample(Sampler, input.Uv).a;
	float3 diffuse = DiffuseMap.Sample(Sampler, input.Uv).rgb;
	
	return float4(HSVtoRGB(uvColor)*0.2f + diffuse, alpha);
}


//6.Edge
float2 PixelSize;
float4 LightColor;
float Intensity;
float4 PS_Edge(VertexOutput input) : SV_TARGET0
{
	float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
	float3 top = DiffuseMap.Sample(Sampler,input.Uv + float2(0, -PixelSize.y)).rgb;
	
	float3 edge = diffuse.rgb - top;
    float EdotD = dot(edge, float3(1, 1, 1)) * Intensity;
	
	return diffuse + float4(EdotD, EdotD, EdotD,diffuse.a) * LightColor;
}

//7. Outline
matrix World2;
VertexOutput VS_Outline(VertexTexture input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World2);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Uv = input.Uv;
    
    return output;
}
float4 PS_Outline(VertexOutput input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    float4 outline = float4(LightColor.rgb, diffuse.a);
    
    return outline;
}

//8.Tone
float4 PS_Tone(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    float3 color = diffuse.rgb * (1 - input.Uv.x) * 3;
    
    return float4(color, diffuse.a);
}

//9.Wiggle
float RunnigTime;
float4 PS_Wiggle(VertexOutput input) : SV_TARGET0
{
    float2 uv = input.Uv;
   //uv.x = uv.x + uv.x * RunnigTime * 0.03f; //시간에 따라 증가
    
   //uv.x = uv.x + cos(uv.x * RunnigTime) * 0.03f; //울렁거리는 느낌
    uv.y = uv.y + sin(uv.y * RunnigTime) * 0.03f;
    
    float4 diffuse = DiffuseMap.Sample(Sampler, uv);

    return diffuse;
}

//10. Vibe
float4 PS_Vibe(VertexOutput input) : SV_TARGET0
{
    float4 vibe = DiffuseMap.Sample(Sampler, input.Uv);
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    
    vibe -= DiffuseMap.Sample(Sampler, input.Uv + sin(RunnigTime) * 0.01f).r;
    vibe += DiffuseMap.Sample(Sampler, input.Uv + sin(RunnigTime) * 0.01f).g;
    vibe += DiffuseMap.Sample(Sampler, input.Uv + sin(RunnigTime) * 0.01f).b;
    
    vibe *= 0.3f; //*0.3 하면 색상이 줄어듬
    
    return diffuse + vibe;
}

//11. Stone
float4 PS_Stone(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    diffuse -= DiffuseMap.Sample(Sampler, input.Uv - 0.001f);
    diffuse += DiffuseMap.Sample(Sampler, input.Uv + 0.001f);
    diffuse.rgb = (diffuse.r + diffuse.g + diffuse.b) *0.3333f;
    
    return diffuse;
}

//12. Grid
VertexOutput VS_Grid(VertexTexture input)
{
    VertexOutput output;
    //output.Position = input.Position;
    output.Position = mul(input.Position, World);
    output.wPosition = output.Position.xyz;
    
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;

    return output;
}

float4 LineColor;
float GridSize;
float Thick;
float4 PS_Grid(VertexOutput input) : SV_target
{
    float3 diffuse = DiffuseMap.Sample(Sampler, input.Uv).rgb;
    float4 lineColor;
    
    float2 grid = input.wPosition.xy / max(GridSize, 1e-6); //(픽셀의 위치 / GridSize)
    grid = frac(grid); //소수점 앞 정수 지우기
    
    float thick = Thick / max(GridSize, 1e-6);
    
    [flatten] //조건문 쓸때 쓰는거
    if(grid.x < thick || grid.y < thick)
        lineColor = LineColor;
    else
        lineColor = float4(0, 0, 0, 0);
    
    
    return float4(diffuse+lineColor.rgb, 1.0f);

}

//13. Rect
float4 PS_Rect(VertexOutput input) : SV_Target
{
    return Color;
}
//Pipe Line 
technique11 T0
{
    //Character
	P_VP(P0, VS, PS_Diffuse)
	P_VP(P1, VS, PS_Mono)
	P_VP(P2, VS, PS_Intensity)
	P_VP(P3, VS, PS_Blue)
	P_VP(P4, VS, PS_Bloom)
	P_VP(P5, VS, PS_Overlay)
	P_VP(P6, VS, PS_Edge)
	P_VP(P7, VS_Outline, PS_Outline)
	//P_VP(P1, FillMode_WireFrame, VP, PS) //와이어프레임

    //Background
	P_VP(P8, VS, PS_Tone)
	P_VP(P9, VS, PS_Wiggle)
	P_VP(P10, VS, PS_Vibe)
	P_VP(P11, VS, PS_Stone)

    //Grid
	P_VP(P12, VS_Grid, PS_Grid)

    //Rect
	P_VP(P13, VS, PS_Rect)
}