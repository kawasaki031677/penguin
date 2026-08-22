#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "options.h"

#define VERSION "0.1.4"
#define PENGUIN_HEIGHT 5
#define PENGUIN_WIDTH 9
#define FRAME_COUNT 2
#define FRAME_DELAY_US 150000

static volatile sig_atomic_t interrupted = 0;
static int animation_active = 0;
static int terminal_width = 80;
static int terminal_height = 24;
static int old_x = 0;
static int old_y = 0;
static int has_previous_frame = 0;

static const char *frames[FRAME_COUNT][PENGUIN_HEIGHT] = {
    {"   _~_", "  (o o)", " /  V  \\", "/(  _  )\\", "  ^^ ^^"},
    {"  _~_", " (o o)", "/  V  \\", "\\(  _  )/", " ^^  ^^"}
};

static void write_sequence(const char *sequence) {
    ssize_t written = write(STDOUT_FILENO, sequence, strlen(sequence));
    (void)written;
}

static void handle_signal(int signal_number) {
    (void)signal_number;
    interrupted = 1;
}

static void show_help(void) {
    puts("penguin - a useless Linux penguin\n\n"
         "Usage:\n  penguin [OPTION]\n\n"
         "Options:\n"
         "  -h, --help       Show this help\n"
         "  -v, --version    Show version\n"
         "      --dance      Make the penguin dance\n"
         "      --run        Make the penguin run\n"
         "      --fly        Make the penguin fly");
}

static void show_penguin(void) {
    puts("   _~_\n  (o o)\n /  V  \\\n/(  _  )\\\n  ^^ ^^\n\nTux is watching you.");
}

static void terminal_size(void) {
    struct winsize size;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 &&
        size.ws_col > 0 && size.ws_row > 0) {
        terminal_width = size.ws_col;
        terminal_height = size.ws_row;
    }
}

static void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

static void erase_frame(void) {
    int row;
    int start;
    int end;

    if (!has_previous_frame) {
        return;
    }

    start = old_x < 0 ? 0 : old_x;
    end = old_x + PENGUIN_WIDTH;
    if (end > terminal_width) {
        end = terminal_width;
    }
    if (start >= end) {
        return;
    }

    for (row = 0; row < PENGUIN_HEIGHT; row++) {
        int y = old_y + row;
        int width;
        if (y < 0 || y >= terminal_height) {
            continue;
        }
        width = end - start;
        move_cursor(start, y);
        while (width-- > 0) {
            putchar(' ');
        }
    }
}

static void draw_visible_line(const char *line, int x, int y) {
    int index;
    int length = (int)strlen(line);
    int first = x < 0 ? 0 : x;
    int last = x + length;

    if (y < 0 || y >= terminal_height || first >= terminal_width || last <= 0) {
        return;
    }
    if (first < 0) {
        first = 0;
    }
    if (last > terminal_width) {
        last = terminal_width;
    }

    move_cursor(first, y);
    for (index = first - x; index < last - x; index++) {
        putchar(line[index]);
    }
}

static void draw_frame(int x, int y, int frame) {
    int row;
    const char *line;

    frame %= FRAME_COUNT;
    if (frame < 0) {
        frame += FRAME_COUNT;
    }
    for (row = 0; row < PENGUIN_HEIGHT; row++) {
        line = frames[frame][row];
        draw_visible_line(line, x, y + row);
    }
    has_previous_frame = 1;
    old_x = x;
    old_y = y;
}

static int start_animation(void) {
    if (!isatty(STDOUT_FILENO)) {
        show_penguin();
        return 0;
    }
    terminal_size();
    write_sequence("\033[?1049h\033[?25l\033[2J\033[H");
    fflush(stdout);
    animation_active = 1;
    return 1;
}

static void finish_animation(void) {
    if (!animation_active) {
        return;
    }
    erase_frame();
    fflush(stdout);
    write_sequence("\033[?25h\033[?1049l");
    fflush(stdout);
    animation_active = 0;
    has_previous_frame = 0;
}

static void pause_frame(void) {
    struct timespec delay = {0, FRAME_DELAY_US * 1000L};
    nanosleep(&delay, NULL);
}

static void dance(void) {
    int frame;
    if (!start_animation()) {
        return;
    }
    for (frame = 0; frame < 12 && !interrupted; frame++) {
        terminal_size();
        erase_frame();
        draw_frame((terminal_width - PENGUIN_WIDTH) / 2,
                   (terminal_height - PENGUIN_HEIGHT) / 2, frame);
        fflush(stdout);
        pause_frame();
    }
    finish_animation();
}

static void run_penguin(void) {
    int x;
    if (!start_animation()) {
        return;
    }
    for (x = -PENGUIN_WIDTH; x <= terminal_width && !interrupted; x++) {
        terminal_size();
        erase_frame();
        draw_frame(x, (terminal_height - PENGUIN_HEIGHT) / 2, 0);
        fflush(stdout);
        pause_frame();
    }
    finish_animation();
}

static void fly(void) {
    int y;
    if (!start_animation()) {
        return;
    }
    for (y = terminal_height; y >= -PENGUIN_HEIGHT && !interrupted; y--) {
        terminal_size();
        erase_frame();
        draw_frame((terminal_width - PENGUIN_WIDTH) / 2, y, 0);
        fflush(stdout);
        pause_frame();
    }
    finish_animation();
}

int main(int argc, char **argv) {
    const char *option = argc > 1 ? argv[1] : "";
    PenguinOption parsed_option = parse_option(option);

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (parsed_option == PENGUIN_OPTION_DEFAULT) {
        show_penguin();
    } else if (parsed_option == PENGUIN_OPTION_HELP) {
        show_help();
    } else if (parsed_option == PENGUIN_OPTION_VERSION) {
        puts("penguin " VERSION);
    } else if (parsed_option == PENGUIN_OPTION_DANCE) {
        dance();
    } else if (parsed_option == PENGUIN_OPTION_RUN) {
        run_penguin();
    } else if (parsed_option == PENGUIN_OPTION_FLY) {
        fly();
    } else {
        fprintf(stderr, "penguin: unknown option: %s\n", option);
        fprintf(stderr, "Try 'penguin --help' for more information.\n");
        return 2;
    }
    return interrupted ? 130 : 0;
}
