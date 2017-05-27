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

//Datatype allows classifications to be stored very efficiently
//Is an array of char *, which is a double char *
//In order to use this struct, you must first define an array of char* on the class
typedef struct {
  my_string *categories;
} Classifier_List;

//Datatype allows to return list of neighbours because you cannont return arrays in c
typedef struct {
  int *neighbour;
} Neighbour_List;

//Datatype is euclidean point
typedef struct point {
  float *dimension;
  //category must be in the categories array
  int category;
} Point;

//Dataset holds all of the points
typedef struct dataset {
  //d - the dimensionality of the dataset
  int dimensionality;
  int num_points;
  Point* points;
} Dataset;

//Distance holds the distance from a point, to another point
typedef struct point_neighour_relationship {
  float distance;
  Point *neighbour_pointer;
} Point_Neighbour_Relationship;

//Since a comparison point is a distinctly different entity to a data point
typedef struct comparision_point {
  float *dimension;
  struct point_neighbour_relationship *neighbour;
} Comparison_Point;

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
float point_distance(Comparison_Point x, Point y, int dimensions) {
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

//Doing a k nearest neighbour search

int knn_search(int k, Comparison_Point compare, Dataset *datapoints) {
  //Warn if k is even
  if (k % 2 == 0) {
    printf("Warning: %d is even. Tie cases have undefined behviour\n", k);
  }

  //Get the euclidean distance to every neighbour,
  for (int i = 0; i < datapoints->num_points; i++) {
    //if the neighbour is closer than the last closest keep it in a distance array
    printf("Point distance: %lf\n", point_distance(compare, datapoints->points[i], datapoints->dimensionality));
  }


  //Keep track of the distance and a pointer to the point in the dataset
  //Take the first k neighbours
  //Find the largest count of the classification. (what to do in a tie?)
  //return the category of the point
  return 2;
}


//Function that takes in a classification integer, and returns a classification string
//Requires a map between the integers and the string in the form of a classification_map datatype
my_string classify(Classifier_List category_map, int category) {
  my_string class = category_map.categories[category];
  return class;
}

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
