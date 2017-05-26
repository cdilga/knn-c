#include "knn.c"
#include "greatest.h"

//Defining tolerances for tests
#define FLOAT_TOLERANCE 0.001

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST distance_3_dimensions(void) {

  float array1[3] = {2.0, 2.0, 2.0};
  point point1 = {array1, 1};

  float array2[3] = {5.0, 5.0, 5.0};
  point point2 = {array2, 0};

  ASSERT_IN_RANGE(5.1962, point_distance(point1, point2, 3), FLOAT_TOLERANCE);
  PASS();
}

TEST distance_10_dimensions(void) {

  float array1[10] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  point point1 = {array1, 1};

  float array2[10] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
  point point2 = {array2, 0};

  ASSERT_IN_RANGE(28.4605, point_distance(point1, point2, 10), FLOAT_TOLERANCE);
  PASS();
}

TEST distance_1_dimension(void) {

  float array1[1] = {3.0};
  point point1 = {array1, 1};

  float array2[1] = {6.0};
  point point2 = {array2, 0};

  ASSERT_IN_RANGE(3.0, point_distance(point1, point2, 1), FLOAT_TOLERANCE);
  PASS();
}

//How do I initialise arrays with the {} curly braces syntax?

//Test the sort function, which will input an array and return an array of
//ordered classification integers only

//Test the creation of an array (neighbours) with distances to every single point,
//taking one point and a dataset
//We need a distance associated with a point

//

//Test ordering of a dataset, by distance

/* Suites can group multiple tests with common setup. */
SUITE(external_suite) {
    RUN_TEST(distance_1_dimension);
    RUN_TEST(distance_3_dimensions);
    RUN_TEST(distance_10_dimensions);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_SUITE(external_suite);
