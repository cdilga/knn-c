#include "knn.c"
#include "greatest.h"

#include "terminal_user_input.h"

//Defining tolerances for tests
#define FLOAT_TOLERANCE 0.001

/* A test runs various assertions, then calls PASS(), FAIL(), or SKIP(). */
TEST distance_3_dimensions(void) {
  float array1[3] = {2.0, 2.0, 2.0};
  Point point1 = {array1, 1, NULL};

  float array2[3] = {5.0, 5.0, 5.0};
  Point point2 = {array2, 0, NULL};

  ASSERT_IN_RANGE(5.1962, point_distance(point1, point2, 3), FLOAT_TOLERANCE);
  PASS();
}

TEST distance_10_dimensions(void) {
  float array1[10] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  Point point1 = {array1, 1, NULL};

  float array2[10] = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
  Point point2 = {array2, 0, NULL};

  ASSERT_IN_RANGE(28.4605, point_distance(point1, point2, 10), FLOAT_TOLERANCE);
  PASS();
}

TEST distance_1_dimension(void) {
  float array1[1] = {3.0};
  Point point1 = {array1, 1, NULL};

  float array2[1] = {6.0};
  Point point2 = {array2, 0, NULL};

  ASSERT_IN_RANGE(3.0, point_distance(point1, point2, 1), FLOAT_TOLERANCE);
  PASS();
}

//How do I initialise arrays with the {} curly braces syntax?

//Test the creation of an array (neighbours) with distances to every single point,
//taking one point and a dataset
//We need a distance associated with a point

//Test k NN search
//Ensure that the returned array contains the correct integers

//Test the nearest 1 neighbour can be found
TEST find_1_nearest_neighbour(void) {
  int k = 1;

  int category = 0;
  //Pass it the stuff it needs, the dataset,
  float dimensions[] = {5};
  Point point1 = {dimensions, category, NULL};

  //Since we've only got a length of 1, just use a pointer straight to the single point
  Point* points = &point1;
  Dataset single_point_dataset = {1, 1, points};

  float comparison_dimensions[] = {3};
  //TODO, fix the comparison point category
  Point comparision_point = {comparison_dimensions, 0, NULL};

  //One point to compare to the rest
  ASSERT_EQ(category, knn_search(k, comparision_point, single_point_dataset, category));
  //See the list of 1 nearest points matches with the rest
}

TEST classify_int(void) {
  //The class integer to be selected
  int class = 0;

  //Using only the minimum 1 categories
  classifier flower_map;
  flower_map.categories = (my_string*)malloc(sizeof(my_string));

  strcpy(flower_map.categories[0].str, "Iris");

  my_string category = classify(flower_map, class);

  ASSERT_STR_EQ("Iris", category.str);
  PASS();
  free(flower_map.categories);
}

/* Suites can group multiple tests with common setup. */
SUITE(external_suite) {
    RUN_TEST(distance_1_dimension);
    RUN_TEST(distance_3_dimensions);
    RUN_TEST(distance_10_dimensions);

    RUN_TEST(classify_int);

    RUN_TEST(find_1_nearest_neighbour);
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_SUITE(external_suite);
