#pragma once
#include <BVH/BVH.h>


GLfloat* findNormal(BvhNodeTree* coords) {

	float3 u = coords->aabb0_max_or_v1 - coords->aabb0_min_or_v0;
	float3 v = coords->aabb1_min_or_v2 - coords->aabb0_min_or_v0;
	GLfloat normal[3];
	normal[0] = (u.vec[1] * v.vec[2]) - (u.vec[2] * v.vec[1]);
	normal[1] = (u.vec[2] * v.vec[0]) - (u.vec[0] * v.vec[2]);
	normal[2] = (u.vec[0] * v.vec[1]) - (u.vec[1] * v.vec[0]);
	return normal;
}