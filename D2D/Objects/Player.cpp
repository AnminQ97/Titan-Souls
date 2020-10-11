#include "stdafx.h"
#include "Player.h"

Player::Player(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	:moveSpeed(0.0f), focusOffset(Width - 100, Height - 100),
	state(PState::start), bDrawBound(false), pass(0)
{
	animation = new Animation();
	bool t = false;
	wstring spriteFile = L"./Player.png";
	wstring shaderFile = L"Effect.fx";

	Clip* clip;

	//start
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 16, 16, 32), 0.1f);
		for(UINT i = 1 ; i < 9 ; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 256+(16*i), 32, 272+(16*i), 48), 0.2f);

		animation->AddClip(clip);
		t = true;
	}
	
	//  <- A D ->
	{
		clip = new Clip(PlayMode::Loop);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 0 + (16 * i), 0, 16 + (16 * i), 16), 0.1f);

		animation->AddClip(clip);
	}
	// <- Idle -> 
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 0, 16, 16), 0.1f);

		animation->AddClip(clip);
	}
	//w Up
	{
		clip = new Clip(PlayMode::Loop);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 0 + (16 * i), 16, 16 + (16 * i), 32), 0.1f);

		animation->AddClip(clip);
	}
	//Up Idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 16, 16, 32), 0.1f);
		animation->AddClip(clip);
	}
	//s Down
	{
		clip = new Clip(PlayMode::Loop);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 0 + (16 * i), 48, 16 + (16 * i), 64), 0.1f);

		animation->AddClip(clip);
	}
	//Down Idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 48, 16, 64), 0.1f);
		animation->AddClip(clip);
	}
	//WA
	{
		clip = new Clip(PlayMode::Loop);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 0 + (16 * i), 112, 16 + (16 * i), 128), 0.1f);

		animation->AddClip(clip);
	}
	//SA
	{
		clip = new Clip(PlayMode::Loop);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 0 + (16 * i), 80, 16 + (16 * i), 96), 0.1f);

		animation->AddClip(clip);
	}
	//Rolling ->
	{
		clip = new Clip(PlayMode::End);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 0, 16, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 , 0, 112, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + 16, 0, 112 + 16, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + 32, 0, 112 + 32, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + 48, 0, 112 + 48, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + 64, 0, 112 + 64, 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + 80, 0, 112 + 80, 16), 0.1f);
		animation->AddClip(clip);
	}
	//Rolling 위
	{
		clip = new Clip(PlayMode::End);
		for (UINT i = 0; i < 6; i++)
			clip->AddFrame(new Sprite(spriteFile, shaderFile, 96 + (16 * i), 16, 112 + (16 * i), 32), 0.1f);

		animation->AddClip(clip);
	}
	animation->Position(position);
	animation->Scale(scale);
	animation->Play(0);
	collider = new Collider();
}

Player::~Player()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(collider);

}

void Player::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	collider->World(animation->GetSprite()->World());
	collider->Update(V, P);
}

void Player::Render()
{
	animation->Pass(pass);

	animation->Render();
	if (bDrawBound == true)
		collider->Render();
}

void Player::Focus(D3DXVECTOR2 * position, D3DXVECTOR2 * size)
{
	*position = animation->Position() + focusOffset;
	*size = D3DXVECTOR2(1, 1);
}

void Player::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Player::Position(D3DXVECTOR2 & position)
{
	animation->Position(position);
}

D3DXVECTOR2 Player::Position()
{
	return animation->Position();
}

void Player::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Player::Scale(D3DXVECTOR2 & scale)
{
	animation->Scale(scale);
}

D3DXVECTOR2 Player::Scale()
{
	return animation->Scale();
}

Vector2 Player::ScaledSize() //화면에 나오는 이미지 사이즈
{
	Vector2 result;
	result.x = animation->TextureSize().x * animation->Scale().x;
	result.y = animation->TextureSize().y * animation->Scale().y;

	return result;
}

Matrix Player::World()
{
	return animation->GetSprite()->World();
}

Sprite * Player::GetSprite()
{
	return animation->GetSprite();
}
