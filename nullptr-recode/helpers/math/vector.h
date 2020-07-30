#pragma once
#include <cmath>

#define deg(a) a * 57.295779513082f

class vec2 {
public:
	vec2() {
		x = 0.f;
		y = 0.f;
	}

	template <typename T>
	vec2(T X, T Y) {
		x = X;
		y = Y;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}


	bool operator==(const vec2& src) {
		return (src.x == x) && (src.y == y);
	}

	bool operator!=(const vec2& src) {
		return (src.x != x) || (src.y != y);
	}

	vec2& operator=(float fl) {
		x = fl;
		y = fl;
		return *this;
	}
	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	vec2& operator*=(const vec2& v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	vec2& operator*=(float fl) {
		x *= fl;
		y *= fl;
		return *this;
	}

	vec2& operator/=(const vec2& v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	vec2& operator/=(float fl) {
		x /= fl;
		y /= fl;
		return *this;
	}

	vec2& operator+=(const vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2& operator+=(float fl) {
		x += fl;
		y += fl;
		return *this;
	}

	vec2& operator-=(const vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	vec2& operator-=(float fl) {
		x -= fl;
		y -= fl;
		return *this;
	}

	vec2 operator-(void) const
	{
		return vec2(-x, -y);
	}
	vec2 operator+(const vec2& v) {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator+(const float v) {
		return vec2(x + v, y + v);
	}
	vec2 operator-(const float v) {
		return vec2(x - v, y - v);
	}
	vec2 operator-(const vec2& v) {
		return vec2(x - v.x, y - v.y);
	}
	vec2 operator*(float fl) {
		return vec2(x * fl, y * fl);
	}
	vec2 operator*(const vec2& v) {
		return vec2(x * v.x, y * v.y);
	}
	vec2 operator/(float fl) {
		return vec2(x / fl, y / fl);
	}
	vec2 operator/(const vec2& v) {
		return vec2(x / v.x, y / v.y);
	}

	float dot(const vec2& vOther) const {
		return (x * vOther.x + y * vOther.y);
	}

	vec2 normalized() {
		vec2 res = *this;
		float l = res.length();
		if (l != 0.0f) {
			res /= l;
		} else {
			res.x = res.y = 0.0f;
		}
		return res;
	}

	float dist_to(const vec2& vOther) {
		vec2 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.length();
	}

	float length() {
		return (float)sqrt(x * x + y * y);
	}
	float length_sqr(void) {
		return (float)(x * x + y * y);
	}

	float x, y;
};

class vec3 {
public:
	vec3() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	template <typename T>
	vec3(T X, T Y, T Z) {
		x = X;
		y = Y;
		z = Z;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}


	bool operator==(const vec3& src) {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	bool operator!=(const vec3& src) {
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	vec3& operator=(float fl) {
		x = fl;
		y = fl;
		z = fl;
		return *this;
	}
	vec3& operator=(const vec3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	vec3& operator*=(const vec3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vec3& operator*=(float fl) {
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	vec3& operator/=(const vec3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	vec3& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	vec3& operator+=(float fl) {
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	vec3& operator-=(const vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	vec3& operator-=(float fl) {
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	vec3 operator-(void) const
	{
		return vec3(-x, -y, -z);
	}
	vec3 operator+(const vec3& v) {
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	vec3 operator+(const vec3& v) const {
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	vec3 operator+(float v) {
		return vec3(x + v, y + v, z + v);
	}
	vec3 operator+(float v) const {
		return vec3(x + v, y + v, z + v);
	}
	vec3 operator-(const vec3& v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	vec3 operator*(float fl) {
		return vec3(x * fl, y * fl, z * fl);
	}
	vec3 operator*(const vec3& v) {
		return vec3(x * v.x, y * v.y, z * v.z);
	}
	vec3 operator/(float fl) {
		return vec3(x / fl, y / fl, z / fl);
	}
	vec3 operator/(float fl) const {
		return vec3(x / fl, y / fl, z / fl);
	}
	vec3 operator/(const vec3& v) {
		return vec3(x / v.x, y / v.y, z / v.z);
	}
	bool operator>(const vec3& v) {
		return x > v.x && y > v.y && z > v.z;
	}
	bool operator>(float v) {
		return x > v && y > v && z > v;
	}
	bool operator<(const vec3& v) {
		return x < v.x&& y < v.y&& z < v.z;
	}
	bool operator<(float v) {
		return x < v && y < v && z < v;
	}

	void zero() {
		x = y = z = 0.0f;
	}

	float dot(const vec3& vOther) const {
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}

	vec3 normalized() {
		vec3 res = *this;
		float l = res.length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	float dist_to(const vec3& vOther) {
		vec3 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.length();
	}

	float length() const {
		return (float)sqrt(x * x + y * y + z * z);
	}

	float length_sqr(void) {
		return (float)(x * x + y * y + z * z);
	}

	float length_2d() const {
		return (float)sqrt(x * x + y * y);
	}

	void cross_product(const vec3& a, const vec3& b, vec3& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	vec3 cross(const vec3& vOther)
	{
		vec3 res;
		cross_product(*this, vOther, res);
		return res;
	}

	vec3 angle(vec3* up = 0) {
		if (!x && !y) return vec3(0, 0, 0);

		float roll = 0;

		if (up) {
			vec3 left = (*up).cross(*this);
			roll = deg(atan2f(left.z, (left.y * x) - (left.x * y)));
		}

		return vec3(deg(atan2f(-z, sqrtf(x * x + y * y))), deg(atan2f(y, x)), roll);
	}

	void mul_add(const vec3& a, const vec3& b, float scalar) {
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	void mul(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	float x, y, z;
};

class __declspec(align(16)) veca : public vec3
{
public:
	veca(void) {};
	veca(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

public:
	explicit veca(const vec3& vOther) {
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
	}

	veca& operator=(const vec3& vOther) {
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;

		return *this;
	}

	veca& operator=(const veca& vOther) {
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;

		return *this;
	}

	float w;
};

class vec4 {
public:
	vec4() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}

	template <typename T>
	vec4(T X, T Y, T Z, T W) {
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}


	bool operator==(const vec4& src) {
		return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
	}

	bool operator!=(const vec4& src) {
		return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
	}

	vec4& operator=(float fl) {
		x = fl;
		y = fl;
		z = fl;
		w = fl;
		return *this;
	}
	vec4& operator=(const vec4& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	vec4& operator*=(const vec4& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec4& operator*=(float fl) {
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	vec4& operator/=(const vec4& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	vec4& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}

	vec4& operator+=(const vec4& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	vec4& operator+=(float fl) {
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	vec4& operator-=(const vec4& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	vec4& operator-=(float fl) {
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	vec4 operator-(void) const
	{
		return vec4(-x, -y, -z, -w);
	}
	vec4 operator+(const vec4& v) {
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	vec4 operator+(const vec4& v) const {
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	vec4 operator-(const vec4& v) const {
		return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	vec4 operator*(float fl) {
		return vec4(x * fl, y * fl, z * fl, w * fl);
	}
	vec4 operator*(const vec4& v) {
		return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	vec4 operator/(float fl) {
		return vec4(x / fl, y / fl, z / fl, w / fl);
	}
	vec4 operator/(const vec4& v) {
		return vec4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	float dot(const vec4& vOther) const {
		return (x * vOther.x + y * vOther.y + z * vOther.z * + w * vOther.w );
	}

	vec4 normalized() {
		vec4 res = *this;
		float l = res.length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = res.w = 0.0f;
		}
		return res;
	}

	float dist_to(const vec4& vOther) {
		vec4 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		delta.z = w - vOther.w;

		return delta.length();
	}

	float length() {
		return (float)sqrt(x * x + y * y + z * z + w * w);
	}
	float length_sqr(void) {
		return (float)(x * x + y * y + z * z + w * w);
	}

	float x, y, z, w;
};