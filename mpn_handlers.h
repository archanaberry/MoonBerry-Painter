#ifndef MPN_HANDLERS_H
#define MPN_HANDLERS_H

#include "mpn.h"

typedef void (*cmd_handler_t)(mpn_state_t *s, char** tokens, int nt);
void mpn_cmd_end(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_wait(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_feedrate(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_stock(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_default_color(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_color(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_move(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_draw(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_drop(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_lift(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_pressure(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_axis(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_rotate(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_bezier(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_poly(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_close(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_hatch(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_crosshatch(mpn_state_t *s, char **tokens, int nt);
void mpn_cmd_scrumble(mpn_state_t *s, char **tokens, int nt);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} mpn_cmd_entry_t;
extern mpn_cmd_entry_t mpn_commands_table[];

#endif // MPN_HANDLERS_H
