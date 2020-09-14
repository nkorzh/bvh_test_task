//Author: Elagin Dmitrii

#pragma once

#include <iostream>
#include <algorithm>

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

	float x() const {
		return v[0];
	}
	float y() const {
		return v[1];
	}
	float operator[](int i) const {
		return v[std::clamp(i, 0, 2)];
	}
	friend std::ostream& operator<< (std::ostream& out, const vec2& point) {
		out << point[0] << ", " << point[1];
		return out;
	}
};

class vec3 {
	void setEqual(float a) {
		v[0] = v[1] = v[2] = a;
	}
public:

	float v[3];

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

	float operator[](int i) const {
		return v[std::clamp(i, 0, 3)];
	}

	vec3 operator-(const vec3& b) {
		return vec3(v[0] - b[0], v[1] - b[1], v[2] - b[2]);
	}													   
	vec3 operator+(const vec3& b) {
		return vec3(v[0] + b[0], v[1] + b[1], v[2] + b[2]);
	}

	float x() const {
		return v[0];
	}
	float y() const {
		return v[1];
	}
	float z() const {
		return v[2];
	}
	friend std::ostream& operator<< (std::ostream& out, const vec3& point) {
		out << point[0] << ", " << point[1] << ", " << point[2] << ", ";
		return out;
	}
};

class float3
{
	float vec[3];
public:
	float3() {
		vec[0] = 0;
		vec[1] = 0;
		vec[2] = 0;
	}

	float3(float a, float b, float c) {
		vec[0] = a;
		vec[1] = b;
		vec[2] = c;
	}

	//


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

	float operator[](int i) const {
		return vec[std::clamp(i, 0, 3)];
	}
	friend std::ostream& operator<< (std::ostream& out, const float3& point) {
		out << point[0] << ", " << point[1] << ", " << point[2] << ", ";
		return out;
	}
};
