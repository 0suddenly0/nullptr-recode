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
        inalizate_int(255, 255, 255, 255);
        inalizate_float(255, 255, 255, 255);
    }

    color(int r, int g, int b, int a) {
        inalizate_char(r, g, b, a);
        inalizate_int(r, g, b, a);
        inalizate_float(r, g, b, a);
    }

    color(float r, float g, float b, float a) {
        inalizate_char(r, g, b, a);
        inalizate_int(r, g, b, a);
        inalizate_float(r, g, b, a);
    }

    color(color clr, int a) {
        inalizate_char(clr.get<int>(return_color::r), clr.get<int>(return_color::g), clr.get<int>(return_color::b), a);
        inalizate_int(clr.get<int>(return_color::r), clr.get<int>(return_color::g), clr.get<int>(return_color::b), a);
        inalizate_float(clr.get<int>(return_color::r), clr.get<int>(return_color::g), clr.get<int>(return_color::b), a);
    }

    color(color clr, float a) {
        inalizate_char(clr.get<float>(return_color::r), clr.get<float>(return_color::g), clr.get<float>(return_color::b), a);
        inalizate_int(clr.get<float>(return_color::r), clr.get<float>(return_color::g), clr.get<float>(return_color::b), a);
        inalizate_float(clr.get<float>(return_color::r), clr.get<float>(return_color::g), clr.get<float>(return_color::b), a);
    }

    void operator=(const color& _color) {
        inalizate_char(_color.color_char[0], _color.color_char[1], _color.color_char[2], _color.color_char[3]);
        inalizate_int(_color.color_int[0], _color.color_int[1], _color.color_int[2], _color.color_int[3]);
        inalizate_float(_color.color_float[0], _color.color_float[1], _color.color_float[2], _color.color_float[3]);
    }

    color& operator=(const float* _color) {
        inalizate_char(_color[0], _color[1], _color[2], _color[3]);
        inalizate_int(_color[0], _color[1], _color[2], _color[3]);
        inalizate_float(_color[0], _color[1], _color[2], _color[3]);

        return *this;
    }

    color& operator=(const int* _color) {
        inalizate_char(_color[0], _color[1], _color[2], _color[3]);
        inalizate_int(_color[0], _color[1], _color[2], _color[3]);
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

    int r() const { return color_int[0]; }
    int g() const { return color_int[1]; }
    int b() const { return color_int[2]; }
    int a() const { return color_int[3]; }

    unsigned char& operator[](int index)
    {
        return (unsigned char&)color_int[index];
    }
    const unsigned char& operator[](int index) const
    {
        return (unsigned char&)color_int[index];
    }

    template<typename  T>
    T& get(return_color type) {
        switch (type)
        {
        case return_color::r:
            if (std::is_same<T, int>::value) return (T&)color_int[0];
            else return (T&)color_float[0];
            break;
        case return_color::g:
            if (std::is_same<T, int>::value) return (T&)color_int[1];
            else return (T&)color_float[1];
            break;
        case return_color::b:
            if (std::is_same<T, int>::value) return (T&)color_int[2];
            else return (T&)color_float[2];
            break;
        case return_color::a:
            if (std::is_same<T, int>::value) return (T&)color_int[3];
            else return (T&)color_float[3];
            break;
        }
    }
    
    void inalizate_int(int r, int g, int b, int a) {
        color_int[0] = std::clamp(r, 0, 255);
        color_int[1] = std::clamp(g, 0, 255);
        color_int[2] = std::clamp(b, 0, 255);
        color_int[3] = std::clamp(a, 0, 255);
    }

    void inalizate_int(float r, float g, float b, float a) {
        color_int[0] = std::clamp(r * 255.f, 0.f, 255.f);
        color_int[1] = std::clamp(g * 255.f, 0.f, 255.f);
        color_int[2] = std::clamp(b * 255.f, 0.f, 255.f);
        color_int[3] = std::clamp(a * 255.f, 0.f, 255.f);
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
private:

    //meme: 3 colors. Maybe fix later
    unsigned char color_char[4];
    int color_int[4];
    float color_float[4];
};
