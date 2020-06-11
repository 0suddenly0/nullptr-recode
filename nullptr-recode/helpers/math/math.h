#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include "vector.h"
#include "qangle.h"
#include "matrix.h"
#include "../../sdk/sdk.h"

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

/*class vec2;
class vec3;
class qangle;
class matrix3x4;*/

namespace math {
	void angle_vector(const qangle& angles, vec3& forward);
	void angle_vectors(const qangle& angles, vec3& forward, vec3& right, vec3& up);
	void movement_fix(c_user_cmd* m_Cmd, qangle wish_angle, qangle old_angles);
	void vector_angles(const vec3& forward, qangle& angles);
	void vector_angles(const vec3& forward, vec3& angles);
	float normalize_yaw(float yaw);
	bool screen_transform(const vec3& in, vec2& out);
	bool world2screen(const vec3& in, vec2& out);
	void vector_transform(const vec3& in1, const matrix3x4& in2, vec3& out);
}