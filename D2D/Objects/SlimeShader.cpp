#include "stdafx.h"
#include "SlimeShader.h"

SlimeShader::SlimeShader(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	:moveSpeed(0.0f), bDrawBound(false), y(1.5f), x(1.5f), slimeIdle(true)
{
	animation = new Animation();
	wstring spriteFile = L"./slime.png";
	wstring shaderFile = L"Effect.fx";

	Clip* clip;

	//start
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 3, 172, 122, 235), 0.1f);
		animation->AddClip(clip);
	}
	animation->Position(position);
	animation->Scale(scale);
	animation->Play(0);
	collider = new Collider();
}

SlimeShader::~SlimeShader()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(collider);

}

void SlimeShader::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (slimeIdle)
	{
		x -= 0.1f*Time::Delta();
		y += 0.2f*Time::Delta();
		if (y > 1.56f)slimeIdle = false;
	}
	else
	{
		x += 0.1f*Time::Delta();
		y -= 0.2f*Time::Delta();
		if (x > 1.6f) slimeIdle = true;
	}
	animation->Scale(x, y);
	
	animation->Update(V, P);
	collider->World(animation->GetSprite()->World());
	collider->Update(V, P);
}

void SlimeShader::Render()
{
	animation->Render();
	if (bDrawBound == true)
		collider->Render();
}


void SlimeShader::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void SlimeShader::Position(D3DXVECTOR2 & position)
{
	animation->Position(position);
}

D3DXVECTOR2 SlimeShader::Position()
{
	return animation->Position();
}

void SlimeShader::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void SlimeShader::Scale(D3DXVECTOR2 & scale)
{
	animation->Scale(scale);
}

D3DXVECTOR2 SlimeShader::Scale()
{
	return animation->Scale();
}

Vector2 SlimeShader::ScaledSize() //화면에 나오는 이미지 사이즈
{
	Vector2 result;
	result.x = animation->TextureSize().x * animation->Scale().x;
	result.y = animation->TextureSize().y * animation->Scale().y;

	return result;
}

Matrix SlimeShader::World()
{
	return animation->GetSprite()->World();
}

Sprite * SlimeShader::GetSprite()
{
	return animation->GetSprite();
}
