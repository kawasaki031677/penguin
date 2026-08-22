#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>

#include <cmocka.h>

#include "options.h"

static void test_default_option(void **state)
{
    (void)state;
    assert_int_equal(parse_option(NULL), PENGUIN_OPTION_DEFAULT);
    assert_int_equal(parse_option(""), PENGUIN_OPTION_DEFAULT);
}

static void test_help_options(void **state)
{
    (void)state;
    assert_int_equal(parse_option("-h"), PENGUIN_OPTION_HELP);
    assert_int_equal(parse_option("--help"), PENGUIN_OPTION_HELP);
}

static void test_version_options(void **state)
{
    (void)state;
    assert_int_equal(parse_option("-v"), PENGUIN_OPTION_VERSION);
    assert_int_equal(parse_option("--version"), PENGUIN_OPTION_VERSION);
}

static void test_animation_options(void **state)
{
    (void)state;
    assert_int_equal(parse_option("--dance"), PENGUIN_OPTION_DANCE);
    assert_int_equal(parse_option("--run"), PENGUIN_OPTION_RUN);
    assert_int_equal(parse_option("--fly"), PENGUIN_OPTION_FLY);
}

static void test_unknown_option(void **state)
{
    (void)state;
    assert_int_equal(parse_option("--unknown"), PENGUIN_OPTION_UNKNOWN);
    assert_int_equal(parse_option("-x"), PENGUIN_OPTION_UNKNOWN);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_default_option),
        cmocka_unit_test(test_help_options),
        cmocka_unit_test(test_version_options),
        cmocka_unit_test(test_animation_options),
        cmocka_unit_test(test_unknown_option),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
