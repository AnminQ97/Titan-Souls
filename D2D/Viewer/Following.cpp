#include "stdafx.h"
#include "Following.h"

Following::Following(IFollowing * focus)
	: focus(focus)
{
}

Following::~Following()
{
}

void Following::Update()
{
	if (focus == NULL)
		return;

	Vector2 location, size;
	focus->Focus(&location, &size);

	location.x -= (float)Width * 0.5f;
	location.y -= (float)Height * 0.5f;

	location.x += size.x*0.5;
	location.y += size.y*0.5;


	if (location.x > 500)
	{
		location.x += Math::Random(-2, 2);
		location.y += Math::Random(-2, 2);
	}
	
	position = location;

	__super::Update();
}

void Following::Change(IFollowing * focus)
{
	this->focus = focus;
}

void Following::Shake(Vector2 vec)
{
	vec.x += Math::Random(-2, 2);
	vec.y += Math::Random(-2, 2);
	position = vec;
}

