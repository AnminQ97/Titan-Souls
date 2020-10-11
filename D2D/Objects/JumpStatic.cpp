#include "stdafx.h"
#include "JumpStatic.h"

JumpStatic::JumpStatic(Vector2 position, Vector2 scale)
	:Player(position,scale), bJump(false), groundY(0)
{

}

JumpStatic::~JumpStatic()
{

}

void JumpStatic::Update(Matrix & V, Matrix & P)
{
	Vector2 position = animation->Position();
	bool bMove = false; //거짓 = 정지상태 

	float speed = 160.0f;
	float gravity = -0.15f; //중력
	float jumpAccel = 0.25f; //점프 속도

	if (bJump == false) //땅에 있을때
	{
		groundY = position.y; //땅에 있을때 플레이어 y좌표 저장
		moveSpeed.x = 0.0f; //속도 다시 초기화
		moveSpeed.y = 0.0f; //속도 다시 초기화

		//방향키
		if (Key->Down(VK_SPACE))
		{
			bJump = true;
			bMove = true;
			state = PState::Up;
			moveSpeed.y = gravity; //중력 저장
		}

		if (Key->Press('D')) //오른쪽
		{
			bMove = true;
			moveSpeed.x = speed;
			if (bJump == false) 
				state = PState::Walk;
			else
				state = PState::Up; 
			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('A'))
		{
			bMove = true;
			moveSpeed.x = -speed;
			if (bJump == false) 
				state = PState::Walk;
			else
				state = PState::Up;
			animation->RotationDegree(0, 180, 0);
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

		if (Key->Press('W'))
		{
			bMove = true;
			if (bJump == false) 
			{
				moveSpeed.y = speed;
				state = PState::Up;
			}
			else
			{ 
				state = PState::Up;
			}
			animation->RotationDegree(0, 0, 0);
		}
		else if (Key->Press('S'))
		{
			bMove = true;
			if (bJump == false) 
			{
				moveSpeed.y = -speed;
				state = PState::Down;
			}
			else
			{
				state = PState::Up;
			}
		}
		if (Key->Up('W'))
		{
			state = PState::UpIdle;
			animation->RotationDegree(0, 180, 0);
		}
		if (Key->Up('S'))
		{
			state = PState::DownIdle;
			animation->RotationDegree(0, 180, 0);
		}
		
	

	}
	else if (bJump == true) //점프 됐을때
	{
		//중력		//-0.15 + 0.25 * Time::Delta();
		moveSpeed.y += jumpAccel * Time::Delta();
		position.y -= moveSpeed.y;  //플레이어 - 중력

		//일정 높이에 모션 변경
		//점프 시작
		if (moveSpeed.y < 0.0f) 
		{
			bMove = true;
			state = PState::Up;
		}
		//떨어지는 모션
		if (moveSpeed.y > 0.0f) 
		{
			bMove = true;
			state = PState::WA;
		}
		//착지
		if (moveSpeed.y > 0 && position.y <= groundY)
		{
			bJump = false;
			position.y = groundY;
		}
	}

	position += moveSpeed * Time::Delta();

	
	animation->Play((UINT)state);
	animation->Position(position);
	animation->Update(V, P);
}
