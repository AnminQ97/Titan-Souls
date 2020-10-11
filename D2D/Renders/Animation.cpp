#include "stdafx.h"
#include "Animation.h"

Animation::Animation()
	: currentClip(-1), position(0, 0), scale(1, 1), rotation(0, 0, 0)
{
}

Animation::~Animation()
{
	for (Clip* clip : clips)
		SAFE_DELETE(clip);
}

void Animation::AddClip(Clip * clip)
{
	clips.push_back(clip);
}

void Animation::Play(UINT clipNumber)
{
	if (clipNumber == currentClip)
		return;

	if (clipNumber > -1)
		clips[currentClip]->Stop();

	currentClip = clipNumber;
	clips[currentClip]->Play();

}

void Animation::Stop()
{
	if (currentClip > -1)
		clips[currentClip]->Stop();
}

void Animation::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Animation::Position(D3DXVECTOR2 vec)
{
	position = vec;
}

void Animation::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));

}

void Animation::Scale(D3DXVECTOR2 vec)
{
	scale = vec;

}

void Animation::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3( x, y, z));
}

void Animation::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;
}

void Animation::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Animation::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

D3DXVECTOR3 Animation::RotationDegree()
{
	D3DXVECTOR3 vec = Rotation();
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
}

D3DXVECTOR2 Animation::TextureSize()
{
	if (currentClip < 0)
		return D3DXVECTOR2(0, 0);

	return clips[currentClip]->TextureSize();
}

void Animation::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (currentClip < 0)
		return;

	clips[currentClip]->Position(position);
	clips[currentClip]->Scale(scale);
	clips[currentClip]->Rotation(rotation);

	clips[currentClip]->Update(V, P);
}

void Animation::Render()
{
	if (currentClip < 0)
		return;

	clips[currentClip]->Render();
}

Clip * Animation::GetClip()
{
	if (currentClip < 0)
		return NULL;

	return clips[currentClip];
}

Sprite * Animation::GetSprite()
{
	return GetClip()->CurrentSprite();
}

void Animation::Pass(UINT val)
{
	for (Clip* clip : clips)
		clip->Pass(val);
}
