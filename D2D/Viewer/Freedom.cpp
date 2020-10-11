#include "stdafx.h"
#include "Freedom.h"

Freedom::Freedom(float speed)
	: speed(speed)
{
	
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Key->Press(VK_LEFT))
		Move(D3DXVECTOR2(-1, 0) * speed);
	else if (Key->Press(VK_RIGHT))
		Move(D3DXVECTOR2(+1, 0) * speed);

	if (Key->Press(VK_UP))
		Move(D3DXVECTOR2(0, 1) * speed);
	else if (Key->Press(VK_DOWN))
		Move(D3DXVECTOR2(0, -1) * speed);

	__super::Update();
}

void Freedom::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Freedom::Position(D3DXVECTOR2 & vec)
{
	position = vec;
}

void Freedom::Move(D3DXVECTOR2 translation)
{
	position += translation * Time::Delta();
}
