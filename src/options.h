#ifndef PENGUIN_OPTIONS_H
#define PENGUIN_OPTIONS_H

typedef enum {
    PENGUIN_OPTION_DEFAULT,
    PENGUIN_OPTION_HELP,
    PENGUIN_OPTION_VERSION,
    PENGUIN_OPTION_DANCE,
    PENGUIN_OPTION_RUN,
    PENGUIN_OPTION_FLY,
    PENGUIN_OPTION_UNKNOWN
} PenguinOption;

PenguinOption parse_option(const char *option);

#endif
