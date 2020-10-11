#include "stdafx.h"
#include "Slime.h"

Slime::Slime(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	:moveSpeed(0.0f),size(0),state(EState::Idle), bDrawBound(false), pass(0),SlimeShader(position,scale)
{
	animation = new Animation();
	wstring spriteFile = L"./slime.png";
	wstring shaderFile = L"Effect.fx";

	Clip* clip;

	//start
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 4, 0, 123, 127), 0.1f);
		animation->AddClip(clip);
	}
	animation->Position(position);
	animation->Scale(scale);
	animation->Play(0);
	collider = new Collider();
}

Slime::~Slime()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(collider);

}

void Slime::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{	

	/*if(slimeIdle)
	{
		x -= 0.1f*Time::Delta();
		y += 0.2f*Time::Delta();
		if (y > 1.65f)slimeIdle = false;
	}
	else
	{
		x += 0.1f*Time::Delta();
		y -= 0.2f*Time::Delta();
		if (x > 1.65f) slimeIdle = true;
	}*/
	animation->Scale(x, y);

	animation->Update(V, P);
	collider->World(animation->GetSprite()->World());
	collider->Update(V, P);
}

void Slime::Render()
{
	animation->Pass(pass);

	animation->Render();
	if (bDrawBound == true)
		collider->Render();
}


void Slime::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Slime::Position(D3DXVECTOR2 & position)
{
	animation->Position(position);
}

D3DXVECTOR2 Slime::Position()
{
	return animation->Position();
}

void Slime::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Slime::Scale(D3DXVECTOR2 & scale)
{
	animation->Scale(scale);
}

D3DXVECTOR2 Slime::Scale()
{
	return animation->Scale();
}

Vector2 Slime::ScaledSize() //화면에 나오는 이미지 사이즈
{
	Vector2 result;
	result.x = animation->TextureSize().x * animation->Scale().x;
	result.y = animation->TextureSize().y * animation->Scale().y;

	return result;
}

Matrix Slime::World()
{
	return animation->GetSprite()->World();
}

Sprite * Slime::GetSprite()
{
	return animation->GetSprite();
}
