#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <unistd.h>

#include "file_input_output.h"
#include "terminal_user_input.h"

//Datatype is euclidean point
typedef struct {
  int dimensions;
  int *element;
} euclidean_point;

//Apparently C doesn't have boolean types
typedef enum {
  false,
  true
} bool;

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

  //for each element in each, get the squared difference
  for (int i = 0; i < x.dimensions; i++) {

  }

  //sum this squared difference

  //get this sum and find the square root
  return dist;
}

int main () {
  int array1[2] = {2, 2};
  euclidean_point point1 = {2, array1};

  int array2[2] = {3, 2};
  euclidean_point point2 = {2, array2};

  printf("Distance: %lf", distance(point1, point2));
  return 0;
}
