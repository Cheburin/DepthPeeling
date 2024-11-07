//Texture2D shadowMap : register( t0 );
Texture2DMS<float4> map : register( t0 );

SamplerState linearSampler : register( s0 );

ThreeTargets PS(in float4 color : TEXCOORD0)
{ 
   ThreeTargets output;

   output.target0 = float4( 0, 0, 0, 1.0 );

   output.target1 = float4( 0, 0, 0, 1.0 );

   output.target2 = color;

   return  output;
}

float4 CONST_COLOR(in float4 tex : TEXCOORD0):SV_TARGET
{ 
   float4 clip_pos = tex / tex.w;

   float2 tx = float2(clip_pos.x*0.5+0.5,clip_pos.y*-0.5+0.5);

   float Z = map.Load( int2(tx.x*g_vScreenResolution.x, tx.y*g_vScreenResolution.y), 0 ).x;

   //if( (linearDepth(0.1, 1000, clip_pos.z)) < linearDepth(0.1, 1000, Z) )
   
   //    discard;

   return float4( 0.0, 0.0, 1.0, 1.0 );   
}

float4 CONST_COLOR2(in float4 tex : TEXCOORD0):SV_TARGET
{ 
   return float4( 1.0, 1.0, 1.0, 0.0 );
}

float4 COLOR(in float4 color : TEXCOORD0):SV_TARGET
{ 
   return color;
}

float4 DEPTH(in float4 tex : TEXCOORD0):SV_TARGET
{ 
   return float4(tex.z / tex.w, 0, 0, 0);
}

float4 MIRROW_COLOR(in float4 tex : TEXCOORD0):SV_TARGET
{ 
   float4 pos = tex / tex.w;

   float2 tx = float2(pos.x*0.5+0.5,pos.y*-0.5+0.5);

   return float4(0.2,0.2,0.2,0)+map.Load( int2(tx.x*g_vScreenResolution.x, tx.y*g_vScreenResolution.y), 0 );
}

float4 MAP_COLOR(in float2 tx : TEXCOORD0):SV_TARGET
{ 
   return map.Load( int2(tx.x*g_vScreenResolution.x, tx.y*g_vScreenResolution.y), 0 );
}