#include "TileAnimation.h"

TileAnimation::TileAnimation(int noaw, int noah, float timebeetween)
{
	setAnimation(noaw, noah, timebeetween);
}

void TileAnimation::setAnimation(int noaw, int noah, float timebeetween)
{
	this->noah = noah;
	this->noaw = noaw;
	this->timebeetween = timebeetween;
	cpx = cpy = 1;
	timeToNextFrame = 0;
}

void TileAnimation::update(float dt)
{
	//width / noah
	timeToNextFrame += dt;
	if (timeToNextFrame > timebeetween) {
		timeToNextFrame = 0;
		cpx++;
		if (cpx >= noaw) {
			cpx = 1;
		}
	}
	
}

vec4 TileAnimation::uv()
{
	return vec4((float)cpx, (float)noaw, (float)cpy, (float)noah);
}
