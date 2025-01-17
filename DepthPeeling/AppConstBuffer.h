#pragma once
struct SceneState
{
	DirectX::XMFLOAT4X4    mWorld;                         // World matrix
	DirectX::XMFLOAT4X4    mView;                          // View matrix
	DirectX::XMFLOAT4X4    mProjection;                    // Projection matrix
	DirectX::XMFLOAT4X4    mWorldViewProjection;           // WVP matrix
	DirectX::XMFLOAT4X4    mWorldView;                     // WV matrix
	DirectX::XMFLOAT4X4    mViewProjection;                // VP matrix
	DirectX::XMFLOAT4X4    mInvView;                       // Inverse of view matrix

	DirectX::XMFLOAT4X4    mLightView;                // VP matrix
	DirectX::XMFLOAT4X4    mWorldLightView;                       // Inverse of view matrix
	DirectX::XMFLOAT4X4    mWorldLightViewProjection;                       // Inverse of view matrix
	DirectX::XMFLOAT4X4    mWorldMirrowViewProjection;                       // Inverse of view matrix

	DirectX::XMFLOAT4      vScreenResolution;              // Screen resolution

	DirectX::XMFLOAT4    viewLightPos;                   //
	DirectX::XMFLOAT4    vTessellationFactor;            // Edge, inside, minimum tessellation factor and 1/desired triangle size
	DirectX::XMFLOAT4    vDetailTessellationHeightScale; // Height scale for detail tessellation of grid surface
	DirectX::XMFLOAT4    vGridSize;                      // Grid size

	DirectX::XMFLOAT4    vDebugColorMultiply;            // Debug colors
	DirectX::XMFLOAT4    vDebugColorAdd;                 // Debug colors

	DirectX::XMFLOAT4    vFrustumPlaneEquation[4];       // View frustum plane equations
};