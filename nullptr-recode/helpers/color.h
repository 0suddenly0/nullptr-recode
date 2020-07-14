#pragma once
#include <algorithm>

enum class return_color{
    r = 0,
    g = 1,
    b = 2,
    a = 3
};

class color {
public:
    color() {
        inalizate_char(255, 255, 255, 255);
        inalizate_float(255, 255, 255, 255);
    }

    color(int r, int g, int b, int a) {
        inalizate_char(r, g, b, a);
        inalizate_float(r, g, b, a);
    }

    color(float r, float g, float b, float a) {
        inalizate_char(r, g, b, a);
        inalizate_float(r, g, b, a);
    }

    color(color clr, int a) {
        inalizate_char(clr.color_char[0], clr.color_char[1], clr.color_char[2], a);
        inalizate_float(clr.color_char[0], clr.color_char[1], clr.color_char[2], a);
    }

    color(color clr, float a) {
        inalizate_char(clr.color_float[0], color_float[1], color_float[2], a);
        inalizate_float(clr.color_float[0], color_float[1], color_float[2], a);
    }

    void operator=(const color& _color) {
        inalizate_char(_color.color_char[0], _color.color_char[1], _color.color_char[2], _color.color_char[3]);
        inalizate_float(_color.color_float[0], _color.color_float[1], _color.color_float[2], _color.color_float[3]);
    }

    color& operator=(const float* _color) {
        inalizate_char(_color[0], _color[1], _color[2], _color[3]);
        inalizate_float(_color[0], _color[1], _color[2], _color[3]);

        return *this;
    }

    color& operator=(const int* _color) {
        inalizate_char(_color[0], _color[1], _color[2], _color[3]);
        inalizate_float(_color[0], _color[1], _color[2], _color[3]);

        return *this;
    }

    bool operator==(const color& rhs) const {
        return (*((int*)this) == *((int*)&rhs));
    }
    bool operator!=(const color& rhs) const {
        return !(operator==(rhs));
    }

    int get_raw() const {
        return *((int*)this);
    }

    void set_raw(int color32) {
        *((int*)this) = color32;
    }

    template<typename T = unsigned char>
    T& r() {
        return std::is_same<T, unsigned char>::value ? (T&)color_char[0] : (T&)color_float[0];
    }

    template<typename T = unsigned char>
    T& g() {
        return std::is_same<T, unsigned char>::value ? (T&)color_char[1] : (T&)color_float[1];
    }

    template<typename T = unsigned char>
    T& b() {
        return std::is_same<T, unsigned char>::value ? (T&)color_char[2] : (T&)color_float[2];
    }

    template<typename T = unsigned char>
    T& a() {
        return std::is_same<T, unsigned char>::value ? (T&)color_char[3] : (T&)color_float[3];
    }
    
    void inalizate_char(int r, int g, int b, int a) {
        color_char[0] = std::clamp(r, 0, 255);
        color_char[1] = std::clamp(g, 0, 255);
        color_char[2] = std::clamp(b, 0, 255);
        color_char[3] = std::clamp(a, 0, 255);
    }

    void inalizate_char(float r, float g, float b, float a) {
        color_char[0] = std::clamp(r * 255.f, 0.f, 255.f);
        color_char[1] = std::clamp(g * 255.f, 0.f, 255.f);
        color_char[2] = std::clamp(b * 255.f, 0.f, 255.f);
        color_char[3] = std::clamp(a * 255.f, 0.f, 255.f);
    }

    void inalizate_float(float r, float g, float b, float a) {
        color_float[0] = std::clamp(r, 0.f, 1.f);
        color_float[1] = std::clamp(g, 0.f, 1.f);
        color_float[2] = std::clamp(b, 0.f, 1.f);
        color_float[3] = std::clamp(a, 0.f, 1.f);
    }

    void inalizate_float(int r, int g, int b, int a) {
        color_float[0] = std::clamp(r / 255.f, 0.f, 1.f);
        color_float[1] = std::clamp(g / 255.f, 0.f, 1.f);
        color_float[2] = std::clamp(b / 255.f, 0.f, 1.f);
        color_float[3] = std::clamp(a / 255.f, 0.f, 1.f);
    }

    unsigned char color_char[4];
    //int color_int[4];
    float color_float[4];
};
