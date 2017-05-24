#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST x_should_equal_1(void) {
    int x = 1;
    ASSERT_EQ(1, x);                              /* default message */
    ASSERT_EQm("yikes, x doesn't equal 1", 1, x); /* custom message */
    /* printf expected and actual values as "%d" if they differ */
    ASSERT_EQ_FMT(1, x, "%d");
    PASS();
}

/* Suites can group multiple tests with common setup. */
SUITE(external_suite) {
    RUN_TEST(x_should_equal_1);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_SUITE(external_suite);
