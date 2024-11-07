#include "GeometricPrimitive.h"
#include "Effects.h"
#include "DirectXHelpers.h"
#include "Model.h"
#include "CommonStates.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SimpleMath.h"

#include "DirectXMath.h"

#include "DXUT.h"

#include <wrl.h>

#include <map>
#include <algorithm>
#include <array>
#include <memory>
#include <assert.h>
#include <malloc.h>
#include <Exception>

#include "ConstantBuffer.h"

#include "AppConstBuffer.h"

using namespace DirectX;

struct EffectShaderFileDef{
	WCHAR * name;
	WCHAR * entry_point;
	WCHAR * shader_ver;
};
class IPostProcess
{
public:
	virtual ~IPostProcess() { }

	virtual void __cdecl Process(_In_ ID3D11DeviceContext* deviceContext, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr) = 0;
};
std::unique_ptr<DirectX::IEffect> createHlslEffect(ID3D11Device* device, std::map<const WCHAR*, EffectShaderFileDef>& fileDef);
std::unique_ptr<IPostProcess> createPostProcess(ID3D11Device* device, std::map<const WCHAR*, EffectShaderFileDef>& fileDef);
inline ID3D11RenderTargetView** renderTargetViewToArray(ID3D11RenderTargetView* rtv1, ID3D11RenderTargetView* rtv2 = 0, ID3D11RenderTargetView* rtv3 = 0){
	static ID3D11RenderTargetView* rtvs[10];
	rtvs[0] = rtv1;
	rtvs[1] = rtv2;
	rtvs[2] = rtv3;
	return rtvs;
};
inline ID3D11ShaderResourceView** shaderResourceViewToArray(ID3D11ShaderResourceView* rtv1, ID3D11ShaderResourceView* rtv2 = 0, ID3D11ShaderResourceView* rtv3 = 0){
	static ID3D11ShaderResourceView* srvs[10];
	srvs[0] = rtv1;
	srvs[1] = rtv2;
	srvs[2] = rtv3;
	return srvs;
};

void floor_set_world_matrix();
void floor_draw(ID3D11DeviceContext* pd3dImmediateContext, IEffect* effect, ID3D11InputLayout* inputLayout, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr);

void teapot_set_light_position();
void teapot_set_world_matrix();
void teapot_draw(ID3D11DeviceContext* pd3dImmediateContext, IEffect* effect, ID3D11InputLayout* inputLayout, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr);

void DrawQuad(ID3D11DeviceContext* pd3dImmediateContext, _In_ IEffect* effect, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr);

class GraphicResources {
public:
	std::unique_ptr<CommonStates> render_states;

	std::unique_ptr<DirectX::IEffect> quad_effect, lambert_none_cull_effect, full_screen_quad_effect;

	std::unique_ptr<DirectX::ConstantBuffer<SceneState> > scene_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pos_tex_normal_layout;

	std::unique_ptr<GeometricPrimitive> teapot;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
};

class SwapChainGraphicResources {
public:
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > depthStencilSRV;
	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView> > depthStencilV;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D> > depthStencilT;

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > colorLayerSRV;
	std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView> > colorLayerV;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D> > colorLayerT;
};