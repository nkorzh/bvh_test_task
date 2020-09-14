//Author: Elagin Dmitrii

#pragma once
#include <BVH/BVH.h>


inline vec3 findNormal(BvhNodeTree* coords) {

	vec3 u = coords->aabb0_max_or_v1 - coords->aabb0_min_or_v0;
	vec3 v = coords->aabb1_min_or_v2 - coords->aabb0_min_or_v0;
	vec3 normal;
	normal.v[0] = (u[1] * v[2]) - (u[2] * v[1]);
	normal.v[1] = (u[2] * v[0]) - (u[0] * v[2]);
	normal.v[2] = (u[0] * v[1]) - (u[1] * v[0]);
	return normal;
}
