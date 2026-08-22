#include <string.h>

#include "options.h"

PenguinOption parse_option(const char *option)
{
    if (option == NULL || strcmp(option, "") == 0) {
        return PENGUIN_OPTION_DEFAULT;
    }
    if (strcmp(option, "-h") == 0 || strcmp(option, "--help") == 0) {
        return PENGUIN_OPTION_HELP;
    }
    if (strcmp(option, "-v") == 0 || strcmp(option, "--version") == 0) {
        return PENGUIN_OPTION_VERSION;
    }
    if (strcmp(option, "--dance") == 0) {
        return PENGUIN_OPTION_DANCE;
    }
    if (strcmp(option, "--run") == 0) {
        return PENGUIN_OPTION_RUN;
    }
    if (strcmp(option, "--fly") == 0) {
        return PENGUIN_OPTION_FLY;
    }
    return PENGUIN_OPTION_UNKNOWN;
}
