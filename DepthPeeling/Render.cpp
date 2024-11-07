#include "main.h"

#include "DXUTgui.h"
#include "SDKmisc.h"

extern GraphicResources * G;

extern SwapChainGraphicResources * SCG;

extern SceneState scene_state;

extern CDXUTTextHelper*                    g_pTxtHelper;

ID3D11ShaderResourceView* null[] = { nullptr, nullptr, nullptr };

namespace Camera{
	void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
}
//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	Camera::OnFrameMove(fTime, fElapsedTime, pUserContext);
}

void RenderText()
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos(2, 0);
	g_pTxtHelper->SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	g_pTxtHelper->DrawTextLine(DXUTGetFrameStats(true && DXUTIsVsyncEnabled()));
	g_pTxtHelper->DrawTextLine(DXUTGetDeviceStats());

	g_pTxtHelper->End();
}

template<class T> inline ID3D11Buffer** constantBuffersToArray(DirectX::ConstantBuffer<T> &cb){
	static ID3D11Buffer* cbs[10];
	cbs[0] = cb.GetBuffer();
	return cbs;
};

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void OnDrawScene(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
void OnPostProccess(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime, void* pUserContext)
{
	pd3dImmediateContext->ClearDepthStencilView(SCG->depthStencilV[0].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

	OnDrawScene(pd3dDevice, pd3dImmediateContext, SCG->colorLayerV[0].Get(), SCG->depthStencilV[1].Get());

	for (int level = 1; level < 4; level++){
		std::swap(SCG->depthStencilT[0], SCG->depthStencilT[1]);
		std::swap(SCG->depthStencilV[0], SCG->depthStencilV[1]);
		std::swap(SCG->depthStencilSRV[0], SCG->depthStencilSRV[1]);

		OnDrawScene(pd3dDevice, pd3dImmediateContext, SCG->colorLayerV[level].Get(), SCG->depthStencilV[1].Get());
	}

	OnPostProccess(pd3dDevice, pd3dImmediateContext, DXUTGetD3D11RenderTargetView(), DXUTGetD3D11DepthStencilView());

	RenderText();
}

void OnPostProccess(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV){
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pd3dImmediateContext->OMSetRenderTargets(1, renderTargetViewToArray(pRTV), pDSV);

	for (int level = 3; level > -1; level--){
		DrawQuad(pd3dImmediateContext, G->full_screen_quad_effect.get(), [=]{
			pd3dImmediateContext->PSSetShaderResources(0, 1, shaderResourceViewToArray(SCG->colorLayerSRV[level].Get()));

			pd3dImmediateContext->OMSetBlendState(G->render_states->AlphaBlend(), Colors::Black, 0xFFFFFFFF);
			pd3dImmediateContext->RSSetState(G->render_states->CullNone());
			pd3dImmediateContext->OMSetDepthStencilState(G->render_states->DepthNone(), 0);
		});
	}

	pd3dImmediateContext->PSSetShaderResources(0, 1, null);
}

void OnDrawScene(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{
	float ClearColor[4] = { 0.05f, 0.05f, 0.05f, 1.0f };
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pd3dImmediateContext->OMSetRenderTargets(1, renderTargetViewToArray(pRTV), pDSV);

	if (true){
		floor_set_world_matrix();

		G->scene_constant_buffer->SetData(pd3dImmediateContext, scene_state);

		floor_draw(pd3dImmediateContext, G->quad_effect.get(), 0, [=]{
			pd3dImmediateContext->PSSetConstantBuffers(0, 1, constantBuffersToArray(*(G->scene_constant_buffer)));

			pd3dImmediateContext->PSSetShaderResources(0, 1, shaderResourceViewToArray(SCG->depthStencilSRV[0].Get()));

			pd3dImmediateContext->OMSetBlendState(G->render_states->Opaque(), Colors::Black, 0xFFFFFFFF);
			pd3dImmediateContext->RSSetState(G->rasterizer_state.Get());
			pd3dImmediateContext->OMSetDepthStencilState(G->render_states->DepthDefault(), 0);
		});
	}

	if (true){
		teapot_set_world_matrix();
		
		teapot_set_light_position();

		G->scene_constant_buffer->SetData(pd3dImmediateContext, scene_state);

		teapot_draw(pd3dImmediateContext, G->lambert_none_cull_effect.get(), G->pos_tex_normal_layout.Get(), [=]{
			pd3dImmediateContext->PSSetConstantBuffers(0, 1, constantBuffersToArray(*(G->scene_constant_buffer)));

			pd3dImmediateContext->PSSetShaderResources(0, 1, shaderResourceViewToArray(SCG->depthStencilSRV[0].Get()));

			pd3dImmediateContext->OMSetBlendState(G->render_states->Opaque(), Colors::Black, 0xFFFFFFFF);
			pd3dImmediateContext->RSSetState(G->rasterizer_state.Get());
			pd3dImmediateContext->OMSetDepthStencilState(G->render_states->DepthDefault(), 0);
		});
	}

	pd3dImmediateContext->PSSetShaderResources(0, 1, null);
}