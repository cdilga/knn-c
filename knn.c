#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <unistd.h>

#include "file_input_output.h"
#include "terminal_user_input.h"

//Datatype is euclidean point
typedef struct {
  int dimensions;
  int *element;
} euclidean_point;

//Datatype that classifies points
typedef struct {
  euclidean_point datapoint;
  classification classification;
}



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
float distance(euclidean_point x, euclidean_point y) {
  float dist = 0;
  //verify we can actually compare the arrays
  if (x.dimensions != y.dimensions) {
    printf("Error: dimensionality mismatch\n");
    //TODO, limit data coupling here by using an alternative control measure
    return -1;
  }

  int sum = 0;
  //for each element in each, get the squared difference
  for (int i = 0; i < x.dimensions; i++) {
    //sum this squared difference
    sum = sum + pow(x.element[i] - y.element[i], 2);
  }
  //get this sum and find the square root
  dist = sqrt(sum);

  return dist;
}

//Loop through all the data, comparing each distance to every other distance
//storing the result in an array

int main () {
  //get the number of categories
  //Set an array to the number of categories
  //Populate the array with a string corresponding to each categories

  //Set the dimensionality of the
  //Enter in a number of datapoints to add
  //To add a datapoint, enter

  int array1[4] = {2, 2, 2, 10};
  euclidean_point point1 = {4, array1};

  int array2[4] = {3, 9, 2, 2};
  euclidean_point point2 = {4, array2};

  printf("Distance: %lf", distance(point1, point2));
  return 0;
}
