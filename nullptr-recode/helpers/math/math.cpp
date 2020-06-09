#include "math.h"
#include <cmath>

namespace math {
	bool world2screen(const vec2& in, vec2& out) {
		return true;
	}
	void vector_transform(const vec3& in1, const matrix3x4& in2, vec3& out) {
		out[0] = in1.dot(vec3(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = in1.dot(vec3(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = in1.dot(vec3(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}

}