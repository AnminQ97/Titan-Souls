#pragma once
#include "Scene.h"

class S22_PostEffectDemo : public Scene
{
public:
	S22_PostEffectDemo(SceneValues* values);
	~S22_PostEffectDemo();
		
	void Update() override;
	void Render() override;

private:
	Sprite* background;
	class Kyoko* player;


	UINT cPass;
	UINT bPass;

	int itemNo;

};