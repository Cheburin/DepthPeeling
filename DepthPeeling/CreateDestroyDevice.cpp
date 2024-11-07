#include "main.h"

#include "DXUTgui.h"
#include "SDKmisc.h"

HWND DXUTgetWindow();

GraphicResources * G;

SceneState scene_state;

std::unique_ptr<Keyboard> _keyboard;
std::unique_ptr<Mouse> _mouse;

CDXUTTextHelper*                    g_pTxtHelper = NULL;
CDXUTDialogResourceManager          g_DialogResourceManager;

HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* device, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	G = new GraphicResources();

	ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
	
	G->render_states = std::make_unique<CommonStates>(device);
	G->scene_constant_buffer = std::make_unique<ConstantBuffer<SceneState> >(device);
	
	_keyboard = std::make_unique<Keyboard>();
	_mouse = std::make_unique<Mouse>();

	HWND hwnd = DXUTgetWindow();

	_mouse->SetWindow(hwnd);

	g_DialogResourceManager.OnD3D11CreateDevice(device, context);

	g_pTxtHelper = new CDXUTTextHelper(device, context, &g_DialogResourceManager, 15);
	//effects
	{
		std::map<const WCHAR*, EffectShaderFileDef> shaderDef;
		shaderDef[L"VS"] = { L"Quad.hlsl", L"BILLBOARD", L"vs_5_0" };
		shaderDef[L"PS"] = { L"Quad.hlsl", L"CONST_COLOR", L"ps_5_0" };

		G->quad_effect = createHlslEffect(device, shaderDef);
	}
	{
		std::map<const WCHAR*, EffectShaderFileDef> shaderDef;
		shaderDef[L"VS"] = { L"lambert.hlsl", L"LIGHTED_MODEL", L"vs_5_0" };
		shaderDef[L"PS"] = { L"lambert.hlsl", L"LAMBERT", L"ps_5_0" };

		G->lambert_none_cull_effect = createHlslEffect(device, shaderDef);
	}
	{
		std::map<const WCHAR*, EffectShaderFileDef> shaderDef;
		shaderDef[L"VS"] = { L"Quad.hlsl", L"VS", L"vs_5_0" };
		shaderDef[L"PS"] = { L"Quad.hlsl", L"MAP_COLOR", L"ps_5_0" };

		G->full_screen_quad_effect = createHlslEffect(device, shaderDef);
	}
	//meshes
	{
		G->teapot = GeometricPrimitive::CreateTeapot(context, 1.0F, 8U, false);
	}
	//input layout
	{
		G->teapot->CreateInputLayout(G->lambert_none_cull_effect.get(), G->pos_tex_normal_layout.ReleaseAndGetAddressOf());
	}
	//rester state /////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;
	desc.DepthBias = 10;
	desc.DepthBiasClamp = 1;
	desc.SlopeScaledDepthBias = 1;

	HRESULT hr = device->CreateRasterizerState(&desc, G->rasterizer_state.ReleaseAndGetAddressOf());

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
{

	delete g_pTxtHelper;

	g_DialogResourceManager.OnD3D11DestroyDevice();

	_mouse = 0;

	_keyboard = 0;

	delete G;
}
