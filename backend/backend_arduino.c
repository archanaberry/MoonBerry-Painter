// backend_arduino.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "mpn_backend.h" // gunakan header dari repo; sesuaikan nama fungsi

static int serial_fd = -1;
static FILE *svg_fp = NULL;
static double curx = 0, cury = 0;
static int pen_down = 0;

int backend_init_serial(const char *devpath, int baudrate) {
    serial_fd = open(devpath, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) return -1;

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) { close(serial_fd); serial_fd = -1; return -2; }

    cfmakeraw(&tty);
    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);
    tty.c_cflag |= CLOCAL | CREAD;
    tcsetattr(serial_fd, TCSANOW, &tty);
    return 0;
}

void backend_close_serial(void) {
    if (serial_fd >= 0) { close(serial_fd); serial_fd = -1; }
}

static int serial_send_line(const char *ln) {
    if (serial_fd < 0) return -1;
    size_t len = strlen(ln);
    if (write(serial_fd, ln, len) != (ssize_t)len) return -2;
    if (write(serial_fd, "\n", 1) != 1) return -3;
    tcdrain(serial_fd); // wait until transmitted
    return 0;
}

/* SVG capture */
int backend_svg_open(const char *fname, double width, double height) {
    svg_fp = fopen(fname, "w");
    if (!svg_fp) return -1;
    fprintf(svg_fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 %.0f %.0f\">\n", width, height);
    return 0;
}

void backend_svg_close(void) {
    if (!svg_fp) return;
    fprintf(svg_fp, "</svg>\n");
    fclose(svg_fp);
    svg_fp = NULL;
}

/* Backend API used by mpn engine */
int backend_move(double x, double y) {
    // record for svg
    if (svg_fp && pen_down) {
        // draw a line segment from cur to new
        fprintf(svg_fp, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"1\" />\n",
                curx, cury, x, y);
    }
    curx = x; cury = y;

    // send to arduino: MOVE x y
    char buf[128];
    snprintf(buf, sizeof(buf), "MOVE %.2f %.2f", x, y);
    return serial_send_line(buf);
}

int backend_pen_up(void) {
    pen_down = 0;
    if (svg_fp) {
        // no-op, just state
    }
    return serial_send_line("PENUP");
}

int backend_pen_down(void) {
    pen_down = 1;
    return serial_send_line("PENDOWN");
}

int backend_flush(void) {
    // optional: send flush or wait
    return 0;
}

/* Optional: set speed */
int backend_set_speed(int v) {
    char buf[64];
    snprintf(buf, sizeof(buf), "SPEED %d", v);
    return serial_send_line(buf);
}
