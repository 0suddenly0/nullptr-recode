#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include "vector.h"
#include "qangle.h"
#include "matrix.h"
#include "../../sdk/sdk.h"

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

#define TICK_INTERVAL			(sdk::global_vars->interval_per_tick)
#define TICKS_TO_TIME(t) (sdk::global_vars->interval_per_tick * (t) )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
/*class vec2;
class vec3;
class qangle;
class matrix3x4;*/

namespace math {
	void clamp_angles(qangle& angles);
	void fix_angles(qangle& angle);
	qangle calc_angle(const vec3& src, const vec3& dst);
	void angle_vector(const qangle& angles, vec3& forward);
	void angle_vectors(const qangle& angles, vec3& forward, vec3& right, vec3& up);
	void movement_fix(c_user_cmd* m_Cmd, qangle wish_angle, qangle old_angles);
	void vector_angles(const vec3& forward, qangle& angles);
	void vector_angles(const vec3& forward, vec3& angles);
	float normalize_yaw(float yaw);
	bool screen_transform(const vec3& in, vec2& out);
	bool world2screen(const vec3& in, vec2& out);
	void vector_transform(const vec3& in1, const matrix3x4& in2, vec3& out);

	template<class T>
	void normalize(T& vec) {
		for (auto i = 0; i < 2; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
}