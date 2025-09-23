#ifndef MOONPAINTER_NUMERICALY_H
#define MOONPAINTER_NUMERICALY_H

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

void mpn_init(mpn_state_t *s);
void mpn_parse_line(mpn_state_t *s, char *line);

#endif // MOONPAINTER_NUMERICALY_H
