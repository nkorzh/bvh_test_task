#pragma once
#include <iostream>


class vec2 {
	float v[2];

	void setEqual(float a) {
		v[0] = v[1] = a;
	}

public:
	vec2(float x, float y) {
		v[0] = x;
		v[1] = y;
	}

	vec2(float a) {
		setEqual(a);
	}

	vec2() {
		setEqual(0);
	}

	float x() {
		return v[0];
	}
	float y() {
		return v[1];
	}
};

class vec3 {
	float v[3];
	void setEqual(float a) {
		v[0] = v[1] = v[2] = a;
	}
public:

	vec3(float x, float y, float z) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	vec3(float a) {
		setEqual(a);
	}

	vec3() {
		setEqual(0);
	}

	float x() {
		return v[0];
	}
	float y() {
		return v[1];
	}
	float z() {
		return v[2];
	}
};

struct float3
{
	float3() {
		vec[0] = 0;
		vec[1] = 0;
		vec[2] = 0;
	}

	float vec[3];

	//
	friend std::ostream& operator<< (std::ostream& out, const float3& point) {
		out << point.vec[0] << ", " << point.vec[1] << ", " << point.vec[2] << ", ";
		return out;
	}

	const float3 operator - (const float3& a) {
		float3 ans;
		ans.vec[0] = this->vec[0] - a.vec[0];
		ans.vec[1] = this->vec[1] - a.vec[1];
		ans.vec[2] = this->vec[2] - a.vec[2];
		return ans;
	}

	const float3 operator + (const float3& a) {
		float3 ans;
		ans.vec[0] = this->vec[0] + a.vec[0];
		ans.vec[1] = this->vec[1] + a.vec[1];
		ans.vec[2] = this->vec[2] + a.vec[2];
		return ans;
	}
};
