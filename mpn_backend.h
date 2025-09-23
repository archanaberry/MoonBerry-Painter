#ifndef MPN_BACKEND_H
#define MPN_BACKEND_H

// Stub hardware interface functions
void mb_move(float x, float y);
void mb_draw(float x, float y);
void mb_lift(float z);
void mb_drop(float z);
void mb_set_feedrate(int f);
void mb_wait(int ms);
void mb_rotate(const char* axis, float angle);
void mb_axis_declare(const char* axis, float radius);
void mb_bezier(float x1, float y1, float x2, float y2, float x3, float y3);
void mb_poly_start(float* pts, int count);
void mb_poly_close(void);
void mb_hatch(float angle, float spacing, float x1, float y1, float x2, float y2);
void mb_crosshatch(float spacing, float x1, float y1, float x2, float y2);
void mb_scrumble(int density, float x1, float y1, float x2, float y2);
void mb_set_color(const char* color);
void mb_stock_color(const char* color);
void mb_set_default_color(const char* color);

#endif // MPN_BACKEND_H
