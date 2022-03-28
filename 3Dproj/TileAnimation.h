#pragma once
#include "Vec.h"

class TileAnimation {
public:
	//noah/w = nr of animation height/width,  
	TileAnimation(int noaw = 1, int noah = 1, float timebeetween = 0.16);
	void setAnimation(int noaw = 1, int noah = 1, float timebeetween = 0.16);
	void update(float dt);
	vec4 uv();
private:
	float cpx, cpy;
	int noah, noaw;
	float timebeetween;
	float timeToNextFrame;
};