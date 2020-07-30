#pragma once
#include <cmath>

class qangle {
public:
    qangle(void) {
        init();
    }

    qangle(float X, float Y, float Z) {
        init(X, Y, Z);
    }

    qangle(const float* clr) {
        init(clr[0], clr[1], clr[2]);
    }

    void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) {
        pitch = ix;
        yaw = iy;
        roll = iz;
    }

    float operator[](int i) const {
        return ((float*)this)[i];
    }

    float& operator[](int i) {
        return ((float*)this)[i];
    }

    qangle& operator+=(const qangle& v) {
        pitch += v.pitch; yaw += v.yaw; roll += v.roll;
        return *this;
    }

    qangle& operator-=(const qangle& v) {
        pitch -= v.pitch; yaw -= v.yaw; roll -= v.roll;
        return *this;
    }

    qangle& operator*=(float fl) {
        pitch *= fl;
        yaw *= fl;
        roll *= fl;
        return *this;
    }

    qangle& operator*=(const qangle& v) {
        pitch *= v.pitch;
        yaw *= v.yaw;
        roll *= v.roll;
        return *this;
    }

    qangle& operator/=(const qangle& v) {
        pitch /= v.pitch;
        yaw /= v.yaw;
        roll /= v.roll;
        return *this;
    }

    qangle& operator+=(float fl) {
        pitch += fl;
        yaw += fl;
        roll += fl;
        return *this;
    }

    qangle& operator/=(float fl) {
        pitch /= fl;
        yaw /= fl;
        roll /= fl;
        return *this;
    }

    qangle& operator-=(float fl) {
        pitch -= fl;
        yaw -= fl;
        roll -= fl;
        return *this;
    }

    qangle& operator=(const qangle& vOther) {
        pitch = vOther.pitch; yaw = vOther.yaw; roll = vOther.roll;
        return *this;
    }

    qangle operator-(void) const {
        return qangle(-pitch, -yaw, -roll);
    }

    qangle operator+(const qangle& v) const {
        return qangle(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
    }

    qangle operator-(const qangle& v) const {
        return qangle(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
    }

    qangle operator*(float fl) const {
        return qangle(pitch * fl, yaw * fl, roll * fl);
    }

    qangle operator*(const qangle& v) const {
        return qangle(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
    }

    qangle operator/(float fl) const {
        return qangle(pitch / fl, yaw / fl, roll / fl);
    }

    qangle operator/(const qangle& v) const {
        return qangle(pitch / v.pitch, yaw / v.yaw, roll / v.roll);
    }

    float length() const {
        return sqrt(pitch * pitch + yaw * yaw + roll * roll);
    }

    float length_sqr(void) const {
        return (pitch * pitch + yaw * yaw + roll * roll);
    }

    bool is_zero(float tolerance = 0.01f) const {
        return (pitch > -tolerance && pitch < tolerance &&
            yaw > -tolerance && yaw < tolerance &&
            roll > -tolerance && roll < tolerance);
    }

    float normalize() const {
        qangle res = *this;
        float l = res.length();
        if (l != 0.0f) {
            res /= l;
        } else {
            res[0] = res[1] = res[2] = 0.0f;
        }
        return l;
    }

    float pitch;
    float yaw;
    float roll;
};

qangle operator*(float lhs, const qangle& rhs) {
    return rhs * lhs;
}
qangle operator/(float lhs, const qangle& rhs) {
    return rhs / lhs;
}
