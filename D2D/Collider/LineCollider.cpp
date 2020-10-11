#include "stdafx.h"
#include "LineCollider.h"

float LineCollider::Cross(Vector2 vec1, Vector2 vec2)
{
	return vec1.x * vec2.y - vec2.x* vec1.y; //외적 //곱하는 순서 중요
}

float LineCollider::CCW(Vector2 vec1, Vector2 vec2)
{
	return Cross(vec1,vec2);
}

float LineCollider::CCW(Vector2 init, Vector2 position1, Vector2 position2)
{
	return CCW(position1 - init, position2 - init);
}

//충돌여부
bool LineCollider::LineIntersect(Vector2 A, Vector2 B, Vector2 C, Vector2 D)
{

	float AB = CCW(A, B, C) * CCW(A, B, D);
	float CD = CCW(C, D, A)*CCW(C, D, B);

	if (AB == 0 && CD == 0) //옆으로 나란히 있는 상황
	{
		if (B < A) swap(A, B);
		if (D < C)swap(C, D);

		return !(B < C || D < A); //선 끼리 떨어져 있을때 충돌이 아니다 
	}
	return AB <= 0 && CD <=0;
}
