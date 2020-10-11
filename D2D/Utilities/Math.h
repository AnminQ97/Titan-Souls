#pragma once
class Math
{
public:
	static const float PI;

	static float Modulo(float val1, float val2);

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	template<typename T> static T Clamp(T value, T min, T max)//템플릿은 몸체구현 헤드에서
	{
		value = value > max ? max : value;
		value = value < min ? min : value;

		return value;
	}

	//보강값
	template<typename T > static T Lerp(T value1, T value2, T t)
	{
		//return (1 - t)*value1 + t * value2;
		return value1 + (value2 - value1) * t;
	}
};