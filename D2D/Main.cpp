#include "stdafx.h"
#include "Systems/Device.h"
#include "Viewer/Freedom.h"
#include "Scenes/Scene.h"

#include "Scenes/S13_JumpStaticDemo.h"


SceneValues* values;
vector<Scene*> scenes;

void InitScene()
{
	values = new SceneValues();
	values->MainCamera = new Freedom();
	D3DXMatrixIdentity(&values->Projection);

	scenes.push_back(new S13_JumpStaticDemo(values));

}

void DestroyScene()
{
	for (Scene* scene : scenes)
		SAFE_DELETE(scene);

	SAFE_DELETE(values);
}

void Update()
{
	values->MainCamera->Update();
	D3DXMatrixOrthoOffCenterLH(&values->Projection, (float)Width * -0.5f, (float)Width * 0.5f, (float)Height * -0.5f, (float)Height * 0.5f,	-1, 1);

	for (Scene* scene : scenes)
		scene->Update();
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor);
	{

		for (Scene* scene : scenes)
			scene->Render();
	}
	ImGui::Render();

	//왼쪽 위 프레임 표시
	DirectWrite::GetDC()->BeginDraw();
	{
		RECT rect = { 0, 0, 500, 200 };
		wstring text = L"FPS : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);
	}
	DirectWrite::GetDC()->EndDraw();

	SwapChain->Present(0, 0);
}
