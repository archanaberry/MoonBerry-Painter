#include "mpn_handlers.h"
#include "mpn_backend.h"
#include <string.h>
#include <stdlib.h>

void mpn_cmd_end(mpn_state_t *s, char **tokens, int nt) {
    s->has_end = 1;
}
void mpn_cmd_wait(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        int ms = atoi(tokens[1]);
        mb_wait(ms);
    }
}
void mpn_cmd_feedrate(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        int f = atoi(tokens[1]);
        s->feedrate = f;
        mb_set_feedrate(f);
    }
}
void mpn_cmd_stock(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        mb_stock_color(tokens[1]);
    }
}
void mpn_cmd_default_color(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        mb_set_default_color(tokens[1]);
    }
}
void mpn_cmd_color(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        mb_set_color(tokens[1]);
    }
}
void mpn_cmd_move(mpn_state_t *s, char **tokens, int nt) {
    if (nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X'));
        float y = atof(tokens[2] + (tokens[2][0]=='Y'));
        mb_move(x, y);
    }
}
void mpn_cmd_draw(mpn_state_t *s, char **tokens, int nt) {
    if (nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X'));
        float y = atof(tokens[2] + (tokens[2][0]=='Y'));
        mb_draw(x, y);
    }
}
void mpn_cmd_drop(mpn_state_t *s, char **tokens, int nt) {
    float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : s->default_drop);
    s->z = z;
    mb_drop(z);
}
void mpn_cmd_lift(mpn_state_t *s, char **tokens, int nt) {
    float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : s->default_lift);
    s->z = z;
    mb_lift(z);
}
void mpn_cmd_pressure(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 2) {
        float v = atof(tokens[1]);
        s->z = v;
        mb_drop(v);
    }
}
void mpn_cmd_axis(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 4 && strcmp(tokens[2], "RADIUS") == 0) {
        float r = atof(tokens[3]);
        mb_axis_declare(tokens[1], r);
    }
}
void mpn_cmd_rotate(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 3) {
        float a = atof(tokens[2]);
        mb_rotate(tokens[1], a);
    }
}
void mpn_cmd_bezier(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 7) {
        float x1 = atof(tokens[1] + (tokens[1][0]=='X'));
        float y1 = atof(tokens[2] + (tokens[2][0]=='Y'));
        float x2 = atof(tokens[3] + (tokens[3][0]=='X'));
        float y2 = atof(tokens[4] + (tokens[4][0]=='Y'));
        float x3 = atof(tokens[5] + (tokens[5][0]=='X'));
        float y3 = atof(tokens[6] + (tokens[6][0]=='Y'));
        mb_bezier(x1, y1, x2, y2, x3, y3);
    }
}
void mpn_cmd_poly(mpn_state_t *s, char **tokens, int nt) {
    if (nt >= 5) {
        int pts = (nt - 1) / 2;
        float coords[pts * 2];
        for (int i = 0; i < pts; i++) {
            coords[2 * i] = atof(tokens[1 + 2 * i] + 1);
            coords[2 * i + 1] = atof(tokens[2 + 2 * i] + 1);
        }
        mb_poly_start(coords, pts);
    }
}
void mpn_cmd_close(mpn_state_t *s, char **tokens, int nt) {
    mb_poly_close();
}
void mpn_cmd_hatch(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 7) {
        float angle = atof(tokens[1]);
        float spacing = atof(tokens[2]);
        float x1 = atof(tokens[3] + (tokens[3][0]=='X'));
        float y1 = atof(tokens[4] + (tokens[4][0]=='Y'));
        float x2 = atof(tokens[5] + (tokens[5][0]=='X'));
        float y2 = atof(tokens[6] + (tokens[6][0]=='Y'));
        mb_hatch(angle, spacing, x1, y1, x2, y2);
    }
}
void mpn_cmd_crosshatch(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 6) {
        float spacing = atof(tokens[1]);
        float x1 = atof(tokens[2] + (tokens[2][0]=='X'));
        float y1 = atof(tokens[3] + (tokens[3][0]=='Y'));
        float x2 = atof(tokens[4] + (tokens[4][0]=='X'));
        float y2 = atof(tokens[5] + (tokens[5][0]=='Y'));
        mb_crosshatch(spacing, x1, y1, x2, y2);
    }
}
void mpn_cmd_scrumble(mpn_state_t *s, char **tokens, int nt) {
    if (nt == 6) {
        int density = atoi(tokens[1]);
        float x1 = atof(tokens[2] + (tokens[2][0]=='X'));
        float y1 = atof(tokens[3] + (tokens[3][0]=='Y'));
        float x2 = atof(tokens[4] + (tokens[4][0]=='X'));
        float y2 = atof(tokens[5] + (tokens[5][0]=='Y'));
        mb_scrumble(density, x1, y1, x2, y2);
    }
}
mpn_cmd_entry_t mpn_commands_table[] = {
    {"END", mpn_cmd_end},
    {"WAIT", mpn_cmd_wait},
    {"F", mpn_cmd_feedrate},
    {"STOCK", mpn_cmd_stock},
    {"DEFAULT_COLOR", mpn_cmd_default_color},
    {"COLOR", mpn_cmd_color},
    {"MOVE", mpn_cmd_move},
    {"DRAW", mpn_cmd_draw},
    {"DROP", mpn_cmd_drop},
    {"LIFT", mpn_cmd_lift},
    {"PRESSURE", mpn_cmd_pressure},
    {"AXIS", mpn_cmd_axis},
    {"ROTATE", mpn_cmd_rotate},
    {"BEZIER", mpn_cmd_bezier},
    {"POLY", mpn_cmd_poly},
    {"CLOSE", mpn_cmd_close},
    {"HATCH", mpn_cmd_hatch},
    {"CROSSHATCH", mpn_cmd_crosshatch},
    {"SCRUMBLE", mpn_cmd_scrumble},
    {NULL, NULL}
};
