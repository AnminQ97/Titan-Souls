#pragma once

class LineCollider
{
public:
	static float Cross(Vector2 vec1, Vector2 vec2); //외적
	//그람슈미트의 정규 직교식 조사
	static float CCW(Vector2 vec1, Vector2 vec2); //내적
	static float CCW(Vector2 init, Vector2 position1, Vector2 position2);

	static bool LineIntersect(Vector2 A, Vector2 B, Vector2 C, Vector2 D); //라인 충돌여부
};
