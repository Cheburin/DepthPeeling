ExpandPosNormalTangetColorTex2d MODEL( in PosNormalTangetColorTex2d i )
{
  ExpandPosNormalTangetColorTex2d output;  
  
  output.pos = float4( i.pos, 1.0 );
  output.normal = i.normal;
  output.tangent = i.tangent;
  output.color = i.color;
  output.tex = i.tex;

  return output;
}   

ClipPosTex2d SIMPLE_MODEL( in PosNormalTex2d i )
{
  ClipPosTex2d output;  
  
  output.clip_pos = mul( float4( i.pos, 1.0 ), g_mWorldViewProjection );
  output.tex = i.tex;

  return output;
} 

ExpandPosNormalTex2d THROWPUT( in PosNormalTex2d i )
{
  ExpandPosNormalTex2d output;  
  
  output.pos =  mul( float4( i.pos, 1.0 ), g_mWorldView );
  output.normal = mul( float4( i.normal, 0.0 ), g_mWorldView ).xyz;
  output.tex = i.tex;

  return output;
}   

ClipPosPosNormalTex2dTex4d LIGHTED_MODEL( in PosNormalTex2d i )
{
  ClipPosPosNormalTex2dTex4d output;  

  output.pos = mul( float4( i.pos, 1.0 ), g_mWorldView ).xyz;

  output.normal = mul( float4( i.normal, 0.0 ), g_mWorldView ).xyz;

  output.tex_0 = i.tex;

  float4 clip_pos = mul( float4( i.pos, 1.0 ), g_mWorldViewProjection );

  output.clip_pos = output.tex_1 = clip_pos;

  return output;
} 

ClipPosTex4d MODEL_DEPTH(in PosNormalTex2d i)
{
  ClipPosTex4d output;

  output.clip_pos = mul( float4( i.pos, 1.0 ), g_mWorldLightViewProjection );

  output.tex = output.clip_pos;

  return output;
} 