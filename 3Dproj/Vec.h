#pragma once
#include <math.h>
#include <array>
#include <DirectXMath.h>
#include <string>
//git


struct vec2 {
	vec2() {
		this->x = 0;
		this->y = 0;
	}
	vec2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	float legth() {
		return (float)sqrt(x * x + y * y);
	}
	vec2 Normalize() {
		float le = this->legth();
		this->x /= le;
		this->y /= le;
		return vec2(x,y);
	}
	float x, y;
	vec2 operator*(float other);
	vec2 operator+(vec2 other);
	vec2 operator-(vec2 other);
	vec2 operator/(vec2 other);
	float dot(vec2 a);
};
struct vec3 {
	vec3(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}
	vec3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	vec3 Normalize();
	vec3 X(const vec3& other);
	float operator*(vec3& other);//dot
	vec3 operator*(float other);
	vec3 mul(float other);
	vec3 mul(vec3 other);
	vec3 operator/(vec3 other);
	vec3 operator+(vec3 other);
	vec3 operator-(vec3 other);
	vec3 mirror();
	bool operator==(vec3& other);
	void operator=(vec3 other);
	void operator=(std::array<float, 3> other);
	const DirectX::XMVECTOR toXMvector();
	float length();
	std::string to_string() {
		//return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
		return  std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
	float x;
	float y;
	float z;
};
struct vec4 {
	vec4() {

	};
	vec4(float a, float b, float c, float d) {
		xyz.x = a;
		xyz.y = b;
		xyz.z = c;
		w = d;
	}
	void Normalize();
	float operator*(vec4& other);
	vec4 operator+(vec4& other);
	vec4 operator-(vec4& other);
	bool operator==(vec4& other);
	void operator=(vec4 other);
	float length();

	vec3 xyz;
	float w;
};
void arrayToVec(std::array<float, 3> arr[3], vec3 vec[3]);
void arrayToVec(float arr[3], vec3 &vec);
void arrayToVec(float arr[2], vec2 &vec);
void arrayToVec(std::array<float, 2> arr[3], vec2 vec[3]);