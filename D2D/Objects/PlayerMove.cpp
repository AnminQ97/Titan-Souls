#include "stdafx.h"
#include "PlayerMove.h"

PlayerMove::PlayerMove(Vector2 position, Vector2 scale)
	:Player(position,scale), roll(false),a(0)
{

}

PlayerMove::~PlayerMove()
{

}

void PlayerMove::Update(Matrix & V, Matrix & P)
{
	Vector2 position = animation->Position();
	bool bMove = false; //거짓 = 정지상태 

	float speed = 160.0f;
	float jumpAccel = 0.25f; //점프 속도

	if (Key->Down(VK_SPACE)) //구르기
	{
		roll = true;
	}
	
	if (roll)
	{
		switch (a)
		{
		case 0:	state = PState::Rolling; break;
		case 1:	state = PState::RollingUp; break;
		}
		if (animation->GetClip()->EndFrame() == true)
		{
			roll = false;
		}
	}
	

	if (roll == false) //땅에 있을때
	{
		moveSpeed.x = 0.0f; //속도 다시 초기화
		moveSpeed.y = 0.0f; //속도 다시 초기화

		
		// 왼 위
		if (Key->Press('W') && Key->Press('A'))
		{
			bMove = true;
			moveSpeed.y = speed;
			moveSpeed.x = -speed;
			state = PState::WA;
			animation->RotationDegree(0, 180, 0);
		}
		//오른 위
		else if (Key->Press('W') && Key->Press('D'))
		{
			bMove = true;
			moveSpeed.y = speed;
			moveSpeed.x = speed;
			state = PState::WA;
			animation->RotationDegree(0, 0, 0);
		}
		// 왼아래
		else if (Key->Press('S') && Key->Press('A'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			moveSpeed.x = -speed;
			state = PState::SA;
			animation->RotationDegree(0, 0, 0);
		}
		//오른 아래
		else if (Key->Press('S') && Key->Press('D'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			moveSpeed.x = speed;
			state = PState::SA;
			animation->RotationDegree(0, 180, 0);
		}
		//----
		//좌우					
		else if (Key->Press('D')) //오른쪽
		{
			bMove = true;
			moveSpeed.x = speed;
			state = PState::Walk;

			a = 0;

			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('A'))
		{
			bMove = true;
			moveSpeed.x = -speed;
			state = PState::Walk;

			a = 0;

			animation->RotationDegree(0, 180, 0);
		}
		//----
		//상하
		else if (Key->Press('W'))
		{
			bMove = true;
			moveSpeed.y = speed;
			state = PState::Up;
			a = 1;
		}
		else if (Key->Press('S'))
		{
			bMove = true;
			moveSpeed.y = -speed;
			state = PState::Down;
		}

		//Idle
		if (Key->Up('W'))
		{
			state = PState::UpIdle;
		}
		if (Key->Up('S'))
		{
			state = PState::DownIdle;
		}
		if (Key->Up('A'))
		{
			state = PState::stop;
			animation->RotationDegree(0, 180, 0);
		}
		if (Key->Up('D'))
		{
			state = PState::stop;
			animation->RotationDegree(0, 0, 0);
		}

	}

	position += moveSpeed * Time::Delta();


	animation->Play((UINT)state);
	animation->Position(position);
	animation->Update(V, P);
}
