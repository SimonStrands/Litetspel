#pragma once
#include <cmath>
//not used in this project
struct Quaternion {
	Quaternion(float a, float b, float c, float d) 
	{
		x = a;
		y = b;
		z = c;
		w = d;
		normalize();
	}
	Quaternion() {
		x = 1;
		y = 1;
		z = 1;
		w = 1;
		normalize();
	}
	void normalize() {
		float mag = (float)sqrt(w * w + x * x + y * y + z * z);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
	Quaternion interpolate(Quaternion a, Quaternion b, float weight) {
		Quaternion theReturn;
		float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		float blendI = 1.f - weight;
		if (dot < 0) {
			theReturn.w = blendI * a.w + weight * -b.w;
			theReturn.x = blendI * a.x + weight * -b.x;
			theReturn.y = blendI * a.y + weight * -b.y;
			theReturn.z = blendI * a.z + weight * -b.z;
		}
		else {
			theReturn.w = blendI * a.w + weight * b.w;
			theReturn.x = blendI * a.x + weight * b.x;
			theReturn.y = blendI * a.y + weight * b.y;
			theReturn.z = blendI * a.z + weight * b.z;
		}
		theReturn.normalize();
		return theReturn;
	}

	float x;
	float y;
	float z;
	float w;
};