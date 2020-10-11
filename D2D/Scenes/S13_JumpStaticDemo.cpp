#include "stdafx.h"
#include "S13_JumpStaticDemo.h"
#include "Viewer/Following.h"
#include "Objects/JumpStatic.h"
#include "Objects/PlayerMove.h"
#include "Objects/Slime.h"
#include "Objects/SlimeShader.h"

S13_JumpStaticDemo::S13_JumpStaticDemo(SceneValues * values)
	: Scene(values)
{
	wstring shaderFile = L"Effect.fx";
	background = new Sprite(L"Stage/city.png", shaderFile);
	//background->Position(0, 350);
	background->Scale(3.0f, 3.5f);

	//화면 모서리 영역
	D2D1_RECT_F rt = 
	{
		Width * -0.5f,
		Height * +0.5f,
		Width * +0.5f,
		Height * -0.5f,

	}; 
	//화면 모서리 영역
	player = new PlayerMove(Vector2(100, 170), Vector2(1.5f, 1.5f));
	shader = new SlimeShader(Vector2(-100, 130), Vector2(1.5f, 1.5f));
	slime = new Slime(shader->Position(), Vector2(1.5f, 1.5f));
	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new Following(player);

}

S13_JumpStaticDemo::~S13_JumpStaticDemo()
{
	SAFE_DELETE(background);
	SAFE_DELETE(player);
	SAFE_DELETE(slime);
}

void S13_JumpStaticDemo::Update()
{
	D3DXMATRIX V = values->MainCamera->View(); //카메라
	D3DXMATRIX P = values->Projection;

	background->Update(V, P);
	player->Update(V, P);
	slime->Update(V, P);
	shader->Update(V, P);

}

void S13_JumpStaticDemo::Render()
{
	ImGui::LabelText("position , speed", "%0.2f, %0.2f", player->Position().x,player->Position().y);
	ImGui::LabelText("position , speed", "%0.2f, %0.2f", slime->Position().x, slime->Position().y);

	background->Render();
	player->Render();
	slime->Render();
	shader->Render();
}
