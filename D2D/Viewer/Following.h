#pragma once
#include "Camera.h"
#include "IFollowing.h"

class Following : public Camera
{
public:
	Following(IFollowing* focus = NULL);
	~Following();

	void Update();
	void Change(IFollowing* focus);

	void Shake(Vector2 vec);

	

private:
	IFollowing* focus;
};