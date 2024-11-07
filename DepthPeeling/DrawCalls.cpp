#include "main.h"

extern GraphicResources * G;

extern SceneState scene_state;

template<class T> inline ID3D11Buffer** constantBuffersToArray(DirectX::ConstantBuffer<T> &cb){
	static ID3D11Buffer* cbs[10];
	cbs[0] = cb.GetBuffer();
	return cbs;
};

void DrawQuad(ID3D11DeviceContext* pd3dImmediateContext, _In_ IEffect* effect,
	_In_opt_ std::function<void __cdecl()> setCustomState){
	effect->Apply(pd3dImmediateContext);
	setCustomState();

	pd3dImmediateContext->IASetInputLayout(nullptr);
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pd3dImmediateContext->Draw(4, 0);
}

//floor///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void floor_set_world_matrix(){
	DirectX::XMMATRIX wvp;

	wvp = DirectX::XMMatrixScaling(5, 5, 5);
	DirectX::XMStoreFloat4x4(&scene_state.mWorld, DirectX::XMMatrixTranspose(wvp));
	wvp = wvp * XMMatrixTranspose(XMLoadFloat4x4(&scene_state.mView));
	DirectX::XMStoreFloat4x4(&scene_state.mWorldView, DirectX::XMMatrixTranspose(wvp));
	wvp = wvp * XMMatrixTranspose(XMLoadFloat4x4(&scene_state.mProjection));
	DirectX::XMStoreFloat4x4(&scene_state.mWorldViewProjection, DirectX::XMMatrixTranspose(wvp));

}

void floor_draw(ID3D11DeviceContext* pd3dImmediateContext, IEffect* effect, ID3D11InputLayout* inputLayout, _In_opt_ std::function<void __cdecl()> setCustomState){
	DrawQuad(pd3dImmediateContext, effect, [=]{
		pd3dImmediateContext->VSSetConstantBuffers(0, 1, constantBuffersToArray(*(G->scene_constant_buffer)));

		setCustomState();
	});
}

//teapot///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void teapot_set_world_matrix(){
	DirectX::XMMATRIX wvp = SimpleMath::Matrix::Identity;

	auto orintation = DirectX::XMMATRIX(
		SimpleMath::Vector3(0, 0, 1).Cross(SimpleMath::Vector3(1, 0, 0)),
		SimpleMath::Vector3(0, 0, 1), 
		SimpleMath::Vector3(1, 0, 0), 
		XMLoadFloat4(&XMFLOAT4(0, 0, 0.4, 1)));

	wvp = orintation;
	DirectX::XMStoreFloat4x4(&scene_state.mWorld, DirectX::XMMatrixTranspose(wvp));
	wvp = wvp * XMMatrixTranspose(XMLoadFloat4x4(&scene_state.mView));
	DirectX::XMStoreFloat4x4(&scene_state.mWorldView, DirectX::XMMatrixTranspose(wvp));
	wvp = wvp * XMMatrixTranspose(XMLoadFloat4x4(&scene_state.mProjection));
	DirectX::XMStoreFloat4x4(&scene_state.mWorldViewProjection, DirectX::XMMatrixTranspose(wvp));
}

void teapot_set_light_position(){
	XMMATRIX view = XMMatrixTranspose(XMLoadFloat4x4(&scene_state.mView));
	auto worldLightPos = XMLoadFloat3(&XMFLOAT3(0, 2, 4));
	DirectX::XMStoreFloat4(&scene_state.viewLightPos, XMVector3TransformCoord(worldLightPos, view));
}

void teapot_draw(ID3D11DeviceContext* pd3dImmediateContext, IEffect* effect, ID3D11InputLayout* inputLayout, _In_opt_ std::function<void __cdecl()> setCustomState){
	G->teapot->Draw(effect, inputLayout, false, false, [=]{
		pd3dImmediateContext->VSSetConstantBuffers(0, 1, constantBuffersToArray(*(G->scene_constant_buffer)));

		setCustomState();
	});
}
