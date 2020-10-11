#pragma once
#include "Scene.h"

class S13_JumpStaticDemo : public Scene
{
public:
	S13_JumpStaticDemo(SceneValues* values);
	~S13_JumpStaticDemo();
		
	void Update() override;
	void Render() override;

private:
	Sprite* background;
	class Player* player;
	class Slime* slime;
	class SlimeShader* shader;
};