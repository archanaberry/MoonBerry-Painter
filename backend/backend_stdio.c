#include "../mpn_backend.h"
#include <stdio.h>

// NOTE: Backend not yet implemented
void mb_move(float x, float y) {
    printf("MOVE to (%.2f, %.2f)\n", x, y);
}
void mb_draw(float x, float y) {
    printf("DRAW to (%.2f, %.2f)\n", x, y);
}
void mb_lift(float z) {
    printf("LIFT to Z=%.2f\n", z);
}
void mb_drop(float z) {
    printf("DROP to Z=%.2f\n", z);
}
void mb_set_feedrate(int f) {
    printf("SET FEEDRATE to %d\n", f);
}
void mb_wait(int ms) {
    printf("WAIT for %d ms\n", ms);
}
void mb_rotate(const char* axis, float a) {
    printf("ROTATE axis %s by %.2f degrees\n", axis, a);
}
void mb_axis_declare(const char* axis, float r) {
    printf("DECLARE axis %s with radius %.2f\n", axis, r);
}
void mb_bezier(float x1, float y1, float x2, float y2, float x3, float y3) {
    printf("BEZIER through (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", x1, y1, x2, y2, x3, y3);
}
void mb_poly_start(float* pts, int count) {
    printf("POLY START with %d points\n", count);
    for (int i = 0; i < count; i++) {
        printf("  Point %d: (%.2f, %.2f)\n", i+1, pts[2*i], pts[2*i+1]);
    }
}
void mb_poly_close(void) {
    printf("POLY CLOSE\n");
}
void mb_hatch(float angle, float spacing, float x1, float y1, float x2, float y2) {
    printf("HATCH at angle %.2f with spacing %.2f in box (%.2f, %.2f) to (%.2f, %.2f)\n", angle, spacing, x1, y1, x2, y2);
}
void mb_crosshatch(float spacing, float x1, float y1, float x2, float y2) {
    printf("CROSSHATCH with spacing %.2f in box (%.2f, %.2f) to (%.2f, %.2f)\n", spacing, x1, y1, x2, y2);
}
void mb_scrumble(int d, float x1, float y1, float x2, float y2) {
    printf("SCRUMBLE with density %d in box (%.2f, %.2f) to (%.2f, %.2f)\n", d, x1, y1, x2, y2);
}
void mb_set_color(const char* c) {
    printf("SET COLOR to %s\n", c);
}
void mb_stock_color(const char* c) {
    printf("STOCK COLOR %s\n", c);
}
void mb_set_default_color(const char* c) {
    printf("SET DEFAULT COLOR to %s\n", c);
}
