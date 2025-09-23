#include "mpn_backend.h"

// NOTE: Backend not yet implemented
void mb_move(float x, float y)         { /* implement stepper X/Y */ }
void mb_draw(float x, float y)         { /* implement drawing move */ }
void mb_lift(float z)                  { /* implement pen lift */ }
void mb_drop(float z)                  { /* implement pen drop */ }
void mb_set_feedrate(int f)            { /* implement speed */ }
void mb_wait(int ms)                   { /* implement delay */ }
void mb_rotate(const char* axis, float a) { /* implement rotary axis */ }
void mb_axis_declare(const char* axis, float r) { /* store axis radius */ }
void mb_bezier(float x1, float y1, float x2, float y2, float x3, float y3) { /* implement bezier segmentation */ }
void mb_poly_start(float* pts, int count) { /* implement polygon start */ }
void mb_poly_close(void)               { /* implement polygon close */ }
void mb_hatch(float angle, float spacing, float x1, float y1, float x2, float y2) { /* implement hatch */ }
void mb_crosshatch(float spacing, float x1, float y1, float x2, float y2) { /* implement crosshatch */ }
void mb_scrumble(int d, float x1, float y1, float x2, float y2) { /* implement scrumble */ }
void mb_set_color(const char* c)       { /* select pen tool */ }
void mb_stock_color(const char* c)     { /* declare available color */ }
void mb_set_default_color(const char* c) { /* set fallback color */ }
