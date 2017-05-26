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
  point_neighbour_relationship *neighbour;
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

//Distance holds the distance from a point, to another point
typedef struct {
  float distance;
  point *neighbour_pointer;
} point_neighbour_relationship

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
//Take in k - warn if k is even
//Take in one point, and compare that point to the k-Nearest Neighbours

//Doing a k nearest neighbour search
//Get the euclidean distance to every neighbour, if the neighbour is closer than the last keep it in a distance array
//Create array of distances, with the point associated with the distance
//Keep track of the distance and a pointer to the point in the dataset
//Take the first k neighbours
//Find the largest count of the classification. (what to do in a tie?)

#ifndef NDEBUG
//Definitions required for the testrunner
GREATEST_MAIN_DEFS();
#endif

//This main function takes commandline arguments
int main (int argc, char **argv) {
  //Wrapped in #ifndef so we can make a release version
  #ifndef NDEBUG
  //Setup required testing
    GREATEST_MAIN_BEGIN();

    //Runs tests from external file specified above
    RUN_SUITE(external_suite);

    //Show results of the testing
    GREATEST_MAIN_END();
  #endif

  //get the number of categories
  //Set an array to the number of categories
  //Populate the array with a string corresponding to each categories

  //Set the dimensionality of the
  //Enter in a number of datapoints to add
  //To add a datapoint, enter

  return 0;
}
