#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include "vector.h"
#include "qangle.h"
#include "matrix.h"
#include "../../sdk/sdk.h"
#include "../../gui/nullptr_gui.h"

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)
#define M_PI 3.14159265358979323846
#define M_RADPI 57.295779513082f
#define CHECK_IF_NON_VALID_NUMBER(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

#define TICK_INTERVAL			(sdk::global_vars->interval_per_tick)
#define TICKS_TO_TIME(t) (sdk::global_vars->interval_per_tick * (t) )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )

namespace math {
	float scale_with_fov(float value);
	void rotate_triangle(std::array<vec2, 3>& points, float rotation); 
	vec2 to_vec2(ImVec2 vec);
	ImVec2 to_imvec2(vec2 vec);
	int clamp(int a, int min, int max);
	float clamp(float a, float min, float max);
	void clamp_angles(qangle& angles);
	void fix_angles(qangle& angle);
	qangle calc_angle(const vec3& src, const vec3& dst);
	vec3 cross_product(const vec3& a, const vec3& b);
	void angle_vector(const qangle& angles, vec3& forward);
	void angle_vectors(const qangle& angles, vec3& forward, vec3& right, vec3& up);
	void movement_fix(c_user_cmd* m_Cmd, qangle wish_angle, qangle old_angles);
	void vector_angles(const vec3& forward, qangle& angles);
	void vector_angles(const vec3& forward, vec3& up, qangle& angles);
	float normalize_yaw(float yaw);
	bool screen_transform(const vec3& in, vec2& out);
	bool world2screen(const vec3& in, vec2& out);
	bool world2screen_offscreen(const vec3& in, vec2& out);
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