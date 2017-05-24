#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <unistd.h>

#include <assert.h>
#include "greatest.h"

#include "file_input_output.h"
#include "terminal_user_input.h"

//Define a testing suite that is external to reduce code in this file
SUITE_EXTERN(external_suite);

//Define the debug level. Defaults to terminal output
#define DEBUG = 1

//Datatype is euclidean point
typedef struct {
  float *dimension;
  //category must be in the categories array
  int category;
} point;

//Dataset holds all of the points
typedef struct {
  //d - the dimensionality of the dataset
  int dimensionality;
  int num_points;
  point* points;
} dataset;

//Apparently C doesn't have boolean types
typedef enum {
  false,
  true
} bool;

int knn_pow(int x, int n) {
  int result = 1;
  for (int i = 0; i < n - 1; i++) {
    result = result * x;
  }
  return result;
}

//Distance
//Return: number with the distance, a float
//Inputs, euclidean point, x and euclidean point y
float point_distance(point x, point y, int dimensions) {
  float dist = 0;

  int sum = 0;

  //for each element in each, get the squared difference
  for (int i = 0; i < dimensions; i++) {
    //sum this squared difference
    sum = sum + pow(x.dimension[i] - y.dimension[i], 2);
  }
  //get this sum and find the square root
  dist = sqrt(sum);

  return dist;
}

//Loop through all the data, comparing each distance to every other distance
//storing the result in an array
//Take in a dataset
//Take in k - warn if k is
//Take in one point, and compare that point to the k-Nearest Neighbours
//Get the euclidean distance to every neighbour
//Sort the neighbours into ascending order
//Take the first k neighbours
//Find the largest count of the classification. (what to do in a tie?)


//Definitions required for the testrunner
GREATEST_MAIN_DEFS();

//This main function takes commandline arguments
int main (int argc, char **argv) {
  //Setup required testing
  GREATEST_MAIN_BEGIN();

  //Runs tests from external file specified above
  RUN_SUITE(external_suite);

  //Show results of the testing
  GREATEST_MAIN_END();

  //get the number of categories
  //Set an array to the number of categories
  //Populate the array with a string corresponding to each categories

  //Set the dimensionality of the
  //Enter in a number of datapoints to add
  //To add a datapoint, enter


  float array1[4] = {2, 2, 2, 10};
  point point1 = {array1, 1};

  float array2[4] = {3, 9, 2, 2};
  point point2 = {array2, 0};

  dataset user_dataset = {4, 2, NULL};

  printf("Distance: %lf", point_distance(point1, point2, user_dataset.dimensionality));
  return 0;
}
