 /*
 * Created: Archana Berry (Owner)
 * Patched by: Archana Berry (Write you name when contribute it!)
 * moonpainter_numericaly.c
 * MoonBerry Painter MPN Parser & Runtime Library v1.0_alpha
 * Implements .mpn v1.5 specification
 * Branding: MoonBerry Painter [My Driver Plot Printer]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MOONBERRY_MPN_VERSION "1.0"
#define MAX_TOKENS 16
#define MAX_LINE_LEN 256

// Structures for state

typedef struct {
    float x, y, z;
    int feedrate;
    char current_color[16];
    float default_lift, default_drop;
    int has_end;
} mpn_state_t;

// Handler functions
typedef void (*cmd_handler_t)(char** tokens, int nt);
static void cmd_end(char **tokens, int nt);
static void cmd_wait(char **tokens, int nt);
static void cmd_feedrate(char **tokens, int nt);
static void cmd_stock(char **tokens, int nt);
static void cmd_default_color(char **tokens, int nt);
static void cmd_color(char **tokens, int nt);
static void cmd_move(char **tokens, int nt);
static void cmd_draw(char **tokens, int nt);
static void cmd_drop(char **tokens, int nt);
static void cmd_lift(char **tokens, int nt);
static void cmd_pressure(char **tokens, int nt);
static void cmd_axis(char **tokens, int nt);
static void cmd_rotate(char **tokens, int nt);
static void cmd_bezier(char **tokens, int nt);
static void cmd_poly(char **tokens, int nt);
static void cmd_close(char **tokens, int nt);
static void cmd_hatch(char **tokens, int nt);
static void cmd_crosshatch(char **tokens, int nt);
static void cmd_scrumble(char **tokens, int nt);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_entry_t;

static cmd_entry_t commands_table[] = {
    {"END", cmd_end},
    {"WAIT", cmd_wait},
    {"F", cmd_feedrate},
    {"STOCK", cmd_stock},
    {"DEFAULT_COLOR", cmd_default_color},
    {"COLOR", cmd_color},
    {"MOVE", cmd_move},
    {"DRAW", cmd_draw},
    {"DROP", cmd_drop},
    {"LIFT", cmd_lift},
    {"PRESSURE", cmd_pressure},
    {"AXIS", cmd_axis},
    {"ROTATE", cmd_rotate},
    {"BEZIER", cmd_bezier},
    {"POLY", cmd_poly},
    {"CLOSE", cmd_close},
    {"HATCH", cmd_hatch},
    {"CROSSHATCH", cmd_crosshatch},
    {"SCRUMBLE", cmd_scrumble},
    {NULL, NULL}
};

static mpn_state_t mpn_state;

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

// Utility: tokenize line
static int tokenize(char* line, char* tokens[]) {
    int count = 0;
    char* tok = strtok(line, " \t\r\n");
    while (tok && count < MAX_TOKENS) {
        tokens[count++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }
    return count;
}

// Main parse_line (no main function, use void parser)
void mpn_parse_line(char* line) {
    char* tokens[MAX_TOKENS];
    int nt = tokenize(line, tokens);
    if (nt == 0) return;

    if (strcmp(tokens[0], "@moonpaint") == 0) {
        // Header: skip or verify version
        return;
    }
    if (strcmp(tokens[0], "END") == 0) {
        mpn_state.has_end = 1;
        return;
    }
    if (strcmp(tokens[0], "WAIT") == 0 && nt == 2) {
        int ms = atoi(tokens[1]); mb_wait(ms); return;
    }
    if (strcmp(tokens[0], "F") == 0 && nt == 2) {
        int f = atoi(tokens[1]); mpn_state.feedrate = f; mb_set_feedrate(f); return;
    }
    if (strcmp(tokens[0], "STOCK") == 0 && nt == 2) {
        mb_stock_color(tokens[1]); return;
    }
    if (strcmp(tokens[0], "DEFAULT_COLOR") == 0 && nt == 2) {
        mb_set_default_color(tokens[1]); return;
    }
    if (strcmp(tokens[0], "COLOR") == 0 && nt == 2) {
        mb_set_color(tokens[1]); return;
    }
    if (strcmp(tokens[0], "MOVE") == 0 && nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X')); 
        float y = atof(tokens[2] + (tokens[2][0]=='Y')); 
        mb_move(x, y); return;
    }
    if (strcmp(tokens[0], "DRAW") == 0 && nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X')); 
        float y = atof(tokens[2] + (tokens[2][0]=='Y')); 
        mb_draw(x, y); return;
    }
    if (strcmp(tokens[0], "DROP") == 0) {
        float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : mpn_state.default_drop);
        mpn_state.z = z; mb_drop(z); return;
    }
    if (strcmp(tokens[0], "LIFT") == 0) {
        float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : mpn_state.default_lift);
        mpn_state.z = z; mb_lift(z); return;
    }
    if (strcmp(tokens[0], "PRESSURE") == 0 && nt == 2) {
        float v = atof(tokens[1]); mpn_state.z = v; mb_drop(v); return;
    }
    if (strcmp(tokens[0], "AXIS") == 0 && nt == 4 && strcmp(tokens[2], "RADIUS") == 0) {
        mb_axis_declare(tokens[1], atof(tokens[3])); return;
    }
    if (strcmp(tokens[0], "ROTATE") == 0 && nt == 3) {
        mb_rotate(tokens[1], atof(tokens[2])); return;
    }
    if (strcmp(tokens[0], "BEZIER") == 0 && nt == 7) {
        mb_bezier(atof(tokens[1]+1), atof(tokens[2]+1), atof(tokens[3]+1), atof(tokens[4]+1), atof(tokens[5]+1), atof(tokens[6]+1)); return;
    }
    if (strcmp(tokens[0], "POLY") == 0 && nt >= 5) {
        int pts = (nt - 1) / 2;
        float coords[pts * 2];
        for (int i = 0; i < pts; i++) coords[2*i] = atof(tokens[1+2*i]+1), coords[2*i+1] = atof(tokens[2+2*i]+1);
        mb_poly_start(coords, pts); return;
    }
    if (strcmp(tokens[0], "CLOSE") == 0) { mb_poly_close(); return; }
    if (strcmp(tokens[0], "HATCH") == 0 && nt == 7) {
        float angle = atof(tokens[2]);
        float spacing = atof(tokens[4]);
        float x1 = atof(tokens[6]); // skipping literal REGION parsing for brevity
        float y1 = atof(tokens[7]);
        float x2 = atof(tokens[8]);
        float y2 = atof(tokens[9]);
        mb_hatch(angle, spacing, x1, y1, x2, y2);
        return;
    }
    if (strcmp(tokens[0], "CROSSHATCH") == 0 && nt == 6) {
        float spacing = atof(tokens[2]);
        float x1 = atof(tokens[4]);
        float y1 = atof(tokens[5]);
        float x2 = atof(tokens[6]);
        float y2 = atof(tokens[7]);
        mb_crosshatch(spacing, x1, y1, x2, y2);
        return;
    }
    if (strcmp(tokens[0], "SCRUMBLE") == 0 && nt == 6) {
        int density = atoi(tokens[2]);
        float x1 = atof(tokens[4]);
        float y1 = atof(tokens[5]);
        float x2 = atof(tokens[6]);
        float y2 = atof(tokens[7]);
        mb_scrumble(density, x1, y1, x2, y2);
        return;
    }
    // Unknown or comment
}

void mpn_init(void) {
    memset(&mpn_state, 0, sizeof(mpn_state));
    strcpy(mpn_state.current_color, "BLACK");
    mpn_state.default_drop = 0.0f;
    mpn_state.default_lift = 1.0f;
    mpn_state.feedrate = 1000;
}

// Handler function implementations
static void cmd_end(char **tokens, int nt) {
    mpn_state.has_end = 1;
}
static void cmd_wait(char **tokens, int nt) {
    if (nt == 2) {
        int ms = atoi(tokens[1]);
        mb_wait(ms);
    }
}
static void cmd_feedrate(char **tokens, int nt) {
    if (nt == 2) {
        int f = atoi(tokens[1]);
        mpn_state.feedrate = f;
        mb_set_feedrate(f);
    }
}
static void cmd_stock(char **tokens, int nt) {
    if (nt == 2) {
        mb_stock_color(tokens[1]);
    }
}
static void cmd_default_color(char **tokens, int nt) {
    if (nt == 2) {
        mb_set_default_color(tokens[1]);
    }
}
static void cmd_color(char **tokens, int nt) {
    if (nt == 2) {
        mb_set_color(tokens[1]);
    }
}
static void cmd_move(char **tokens, int nt) {
    if (nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X'));
        float y = atof(tokens[2] + (tokens[2][0]=='Y'));
        mb_move(x, y);
    }
}
static void cmd_draw(char **tokens, int nt) {
    if (nt >= 3) {
        float x = atof(tokens[1] + (tokens[1][0]=='X'));
        float y = atof(tokens[2] + (tokens[2][0]=='Y'));
        mb_draw(x, y);
    }
}
static void cmd_drop(char **tokens, int nt) {
    float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : mpn_state.default_drop);
    mpn_state.z = z;
    mb_drop(z);
}
static void cmd_lift(char **tokens, int nt) {
    float z = (nt == 2 ? atof(tokens[1] + (tokens[1][0]=='Z')) : mpn_state.default_lift);
    mpn_state.z = z;
    mb_lift(z);
}
static void cmd_pressure(char **tokens, int nt) {
    if (nt == 2) {
        float v = atof(tokens[1]);
        mpn_state.z = v;
        mb_drop(v);
    }
}
static void cmd_axis(char **tokens, int nt) {
    if (nt == 4 && strcmp(tokens[2], "RADIUS") == 0) {
        mb_axis_declare(tokens[1], atof(tokens[3]));
    }
}
static void cmd_rotate(char **tokens, int nt) {
    if (nt == 3) {
        mb_rotate(tokens[1], atof(tokens[2]));
    }
}
static void cmd_bezier(char **tokens, int nt) {
    if (nt == 7) {
        mb_bezier(atof(tokens[1]+1),
            atof(tokens[2]+1),
            atof(tokens[3]+1),
            atof(tokens[4]+1),
            atof(tokens[5]+1),
            atof(tokens[6]+1)
        );
    }
}
static void cmd_poly(char **tokens, int nt) {
    if (nt >= 5) {
        int pts = (nt - 1) / 2;
        float coords[pts * 2];
        for (int i = 0; i < pts; i++) {
            coords[2*i] = atof(tokens[1+2*i]+1);
            coords[2*i+1] = atof(tokens[2+2*i]+1);
        }
        mb_poly_start(coords, pts);
    }
}
static void cmd_close(char **tokens, int nt) {
    mb_poly_close();
}
static void cmd_hatch(char **tokens, int nt) {
    if (nt == 7) {
        float angle = atof(tokens[2]);
        float spacing = atof(tokens[4]);
        float x1 = atof(tokens[6]); // skipping literal REGION parsing for brevity
        float y1 = atof(tokens[7]);
        float x2 = atof(tokens[8]);
        float y2 = atof(tokens[9]);
        mb_hatch(angle, spacing, x1, y1, x2, y2);
    }
}
static void cmd_crosshatch(char **tokens, int nt) {
    if (nt == 6) {
        float spacing = atof(tokens[2]);
        float x1 = atof(tokens[4]);
        float y1 = atof(tokens[5]);
        float x2 = atof(tokens[6]);
        float y2 = atof(tokens[7]);
        mb_crosshatch(spacing, x1, y1, x2, y2);
    }
}
static void cmd_scrumble(char **tokens, int nt) {
    if (nt == 6) {
        int density = atoi(tokens[2]);
        float x1 = atof(tokens[4]);
        float y1 = atof(tokens[5]);
        float x2 = atof(tokens[6]);
        float y2 = atof(tokens[7]);
        mb_scrumble(density, x1, y1, x2, y2);
    }
}

// Stub implementations - to be replaced by actual hardware control
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

/* End of moonpainter_numericaly.c */
