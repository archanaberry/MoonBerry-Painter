 /*
 * Created: Archana Berry (Owner)
 * Patched by: Archana Berry (Write you name when contribute it!)
 * mpn.c
 * MoonBerry Painter MPN Parser & Runtime Library v1.0_alpha
 * Implements .mpn v1.5 specification
 * Branding: MoonBerry Painter [My Driver Plot Printer]
 */

#include "mpn.h"
#include "mpn_handlers.h"
#include <stdio.h>
#include <string.h>

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
void mpn_parse_line(mpn_state_t *s, char* line) {
    char* tokens[MAX_TOKENS];
    int nt = tokenize(line, tokens);
    if (nt == 0) return;

    if (strcmp(tokens[0], "@moonpaint") == 0) {
        // Header: skip or verify version
        return;
    }

    for (mpn_cmd_entry_t *e = mpn_commands_table; e->name; e++) {
        if (strcmp(tokens[0], e->name) == 0) {
            e->handler(s, tokens, nt);
            return;
        }
    }
    if (strcmp(tokens[0], "#"))
        fprintf(stderr, "UNKNOWN COMMAND: %s\n", tokens[0]);
}

void mpn_init(mpn_state_t *s) {
    memset(s, 0, sizeof(*s));
    strcpy(s->current_color, "BLACK");
    s->default_drop = 0.0f;
    s->default_lift = 1.0f;
    s->feedrate = 1000;
}
