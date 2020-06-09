#pragma once
#include "vector.h"
#include "matrix.h"

class vec2;
class vec3;
class matrix3x4;

namespace math {
	bool world2screen(const vec2& in, vec2& out);
	void vector_transform(const vec3& in1, const matrix3x4& in2, vec3& out);
}