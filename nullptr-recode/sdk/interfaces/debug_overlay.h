#pragma once
#include "../../helpers/helpers.h"

class overlay_text_t;

class c_debug_overlay {
public:
    virtual void            add_entity_text_overlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
    virtual void            add_box_overlay(const vec3& origin, const vec3& mins, const vec3& max, qangle const& orientation, int r, int g, int b, int a, float duration) = 0;
    virtual void            add_sphere_overlay(const vec3& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            add_triangle_overlay(const vec3& p1, const vec3& p2, const vec3& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
    virtual void            __unkn() = 0;
    virtual void            add_line_overlay(const vec3& origin, const vec3& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
    virtual void            add_text_overlay(const vec3& origin, float duration, const char* format, ...) = 0;
    virtual void            add_text_overlay(const vec3& origin, int line_offset, float duration, const char* format, ...) = 0;
    virtual void            add_screen_text_overlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
    virtual void            add_swept_box_overlay(const vec3& start, const vec3& end, const vec3& mins, const vec3& max, const qangle& angles, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            add_grid_overlay(const vec3& origin) = 0;
    virtual void            add_coord_frame_overlay(const matrix3x4& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
    virtual int             screen_position(const vec3& point, vec3& screen) = 0;
    virtual int             screen_position(float flXPos, float flYPos, vec3& screen) = 0;
    virtual overlay_text_t* get_first(void) = 0;
    virtual overlay_text_t* get_next(overlay_text_t* current) = 0;
    virtual void            clear_dead_overlays(void) = 0;
    virtual void            clear_all_overlays() = 0;
    virtual void            add_text_verlay_rgb(const vec3& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
    virtual void            add_text_overlay_rgb(const vec3& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
    virtual void            add_line_overlay_alpha(const vec3& origin, const vec3& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
    virtual void            add_box_overlay2(const vec3& origin, const vec3& mins, const vec3& max, qangle const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
    virtual void            purge_text_overlays() = 0;
    virtual void            draw_pill(const vec3& mins, const vec3& max, float& diameter, int r, int g, int b, int a, float duration) = 0;
    void                    add_capsule_overlay(vec3& mins, vec3& maxs, float pillradius, int r, int g, int b, int a, float duration)
    {
        return vfunc<void(__thiscall*)(void*, vec3&, vec3&, float&, int, int, int, int, float)>(this, 24)(this, mins, maxs, pillradius, r, g, b, a, duration);
    } 

private:
    inline void add_text_overlay(const vec3& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) {} /* catch improper use of bad interface. Needed because '0' duration can be resolved by compiler to NULL format string (i.e., compiles but calls wrong function) */
};