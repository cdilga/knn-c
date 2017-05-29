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
  int num_categories;
} Classifier_List;

//Datatype allows to return list of neighbours because you cannont return arrays in c
typedef struct {
  int *category;
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
typedef struct point_neighbour_relationship {
  float distance;
  Point *neighbour_pointer;
} Point_Neighbour_Relationship;

//Since a comparison point is a distinctly different entity to a data point
typedef struct comparision_point {
  float *dimension;
  Point_Neighbour_Relationship *neighbour;
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

  float sum = 0;

  //for each element in each, get the squared difference
  for (int i = 0; i < dimensions; i++) {
    //sum this squared difference
    sum = sum + pow(x.dimension[i] - y.dimension[i], 2);
  }

  //get this sum and find the square root
  dist = sqrt(sum);

  return dist;
}

//Compare two integers
int compare_int(const void *v1, const void *v2) {
  int n1 = *(int*)v1;
  int n2 = *(int*)v2;
  if (n1 - n2 > 1) {
    return 1;
  } else if (n1 - n2 < -1) {
    return -1;
  }
  return n1 - n2;
}
//if value 1 is greater than value 2, positive,
//if equal, 0
//if value 1 less value 2, negative,

//Calculate the mode
int mode(int *values, int num_values) {
  //Sort the array
  int current_counter = 1;
  int max_count = 1;
  int max_index = 0;
  //Count the number of each number
  qsort(values, num_values, sizeof(int), compare_int);
  #ifdef DEBUG
  printf("Values[%d]: %d\n", 0, values[0]);
  #endif
  for (int i = 1; i < num_values; i++) {
    //if this is the same as teh last
    if (values[i-1] == values[i]) {
      //increase the couter
      current_counter += 1;
    } else if (current_counter > max_count) {
      //if the counter is greater than the max counter
      //set the max counter to counter
      max_count = current_counter;

      //update the max_index
      max_index = i - 1;
      #ifdef DEBUG
      printf("Max index updated to %d\n", i - 1);
      #endif

      //set the couter to 0
      current_counter = 0;
    }
    //If it's the last one, and the loop doesn't go through again
    if (current_counter > max_count) {
      //if the counter is greater than the max counter
      //set the max counter to counter
      max_count = current_counter;

      //update the max_index
      max_index = i;
      #ifdef DEBUG
      printf("Max index updated to %d\n", i - 1);
      #endif
    }

    //Keep a reference to an instance of the highest counted number in the array
    #ifdef DEBUG
    printf("Values[%d]: %d\n", i, values[i]);
    #endif
  }

  return values[max_index];
}

//Doing a k nearest neighbour search

int knn_search(int k, Comparison_Point compare, Dataset *datapoints) {
  //Warn if k is even
  if (k % 2 == 0) {
    printf("Warning: %d is even. Tie cases have undefined behviour\n", k);
  }

  //create an array the length of k to put all of the compared points in
  compare.neighbour = (Point_Neighbour_Relationship*)malloc(k*sizeof(Point_Neighbour_Relationship));
  //Initialise everything to 0/null so that we can tell if it's been changed.
  for (int i = 0; i < k; i++) {
    compare.neighbour[i] = (Point_Neighbour_Relationship){0, NULL};
  }

  //Get the euclidean distance to every neighbour,
  for (int i = 0; i < datapoints->num_points; i++) {
    float distance = point_distance(compare, datapoints->points[i], datapoints->dimensionality);

    #ifdef DEBUG
    printf("Point distance: %lf\n", distance);
    #endif

    //if the neighbour is closer than the last, or it's null pointer distance closest keep it in a distance array
    //loop through all of the values for k, and keep the value from the comparison list for the compare point which is update_index.
    float max = 0;
    int update_index = 0;
    for (int j = 0; j < k; j++) {
      if (compare.neighbour[j].neighbour_pointer == NULL) {
        max = compare.neighbour[j].distance;
        update_index = j;
      } else if (compare.neighbour[j].distance > max) {
        max = compare.neighbour[j].distance;
        update_index = j;

        #ifdef DEBUG
        printf("update_index: %d\nNeighbour Distance: %lf\n", j, compare.neighbour[j].distance);
        #endif
      }
    }

    //if the current point distance is less than the largest recorded distance, or if the distances haven't been set
    if (compare.neighbour[update_index].neighbour_pointer == NULL || compare.neighbour[update_index].distance > distance) {
      //Update the distance at update_index
      #ifdef DEBUG
      printf("Neighbour number: %d is null, updating pointer\n", i);
      #endif
      compare.neighbour[i].distance = distance;
      compare.neighbour[i].neighbour_pointer = datapoints->points+i;
    }
    #ifdef DEBUG
    printf("=========================================\n");
    #endif
  }
  //Now find the most frequently occurring neighbour pointer type
  //first get all the neighbour pointer categories and put them into a neighbour list
  int neighbour_categories[k];

  for (int i = 0; i < k; i++) {
    neighbour_categories[i] = compare.neighbour[i].neighbour_pointer->category;

    #ifdef DEBUG
    // printf("Neighbour number: %d\nNeighbour Distance: %lf\nCategory: %d\n++++++++++++++++++++++++++++++++++\n", i, compare.neighbour[i].distance, compare.neighbour[i].neighbour_pointer->category);
    printf("Category: %d\n", neighbour_categories[i]);
    #endif
  }

  //Find the mode of the categories
  //Call fuction with array of int and the length of the array and return the result
  printf("mode of categories: %d\n", mode(neighbour_categories, k));

  //TODO fix this memory leak by doing non-dynamic allocation at some point
  return mode(neighbour_categories, k);
  free(compare.neighbour);
}


//Function that takes in a classification integer, and returns a classification string
//Requires a map between the integers and the string in the form of a classification_map datatype
my_string classify(Classifier_List category_map, int category) {
  my_string class = category_map.categories[category];
  return class;
}

Point read_point_user(int num_dimensions, int num_categories) {
  Point user_point;
  user_point.dimension = (float*)malloc(num_dimensions*sizeof(float));
  for (int i = 0; i < num_dimensions; i++) {
    printf("%dth dimension: ", i);
    user_point.dimension[i] = read_float("");
  }
  user_point.category = read_integer("Enter a category ID");
  return user_point;
}


//Passing by reference is less safe, but as a result of the performance increase
//it is justified
void print_point(Point *point_arg, int dimensions) {
  printf("(");
  int i = 0;
  do {
    if (i > 0) {
      printf(", ");
    }
    printf("%lf", point_arg->dimension[i]);
    i++;
  } while(i < dimensions);
  printf(") %d\n", point_arg->category);
}

//Read in a dataset given the number of categories
Dataset read_dataset_user(int num_categories) {
  //Note only good for small datasets. If items are being added for a huge number
  //this operation would get very expensive. Implemention of a linked list would
  //potentially be beneficial for this calculation
  Dataset user_dataset;
  //Dataset contains
  //Dimensionality
  user_dataset.dimensionality = read_integer("Enter the number of dimensions of your classification data: ");

  //Number of points (dynamically updated for UX)
  user_dataset.num_points = 1;
  user_dataset.points = (Point*)malloc(sizeof(Point));

  bool enter_another = true;
  do {
    user_dataset.points[user_dataset.num_points - 1] = read_point_user(user_dataset.dimensionality, num_categories);
    enter_another = read_boolean("Enter another? [y/n] ");
    if (enter_another) {
      user_dataset.num_points += 1;
      user_dataset.points = (Point*)realloc(user_dataset.points, user_dataset.num_points*sizeof(Point));
    } else {
      break;
    }
  } while(enter_another);

  return user_dataset;
}

//Large dataset shouldn't be copied to support large datasets
void print_dataset(Dataset *dataset_arg) {
  printf("Dataset\nDimensionality: %d\nNumber of Points: %d\n", dataset_arg->dimensionality, dataset_arg->num_points);
  for (int i = 0; i < dataset_arg->num_points; i++) {
    print_point(dataset_arg->points + i, dataset_arg->dimensionality);
  }
}

Classifier_List read_classes_user() {
  //In future, read string and similar calls could be mocked to allow unit testing
  //Since the framework already exists for terminal_user input, this could be expanded
  Classifier_List classes;
  classes.categories = (my_string*)malloc(sizeof(my_string));
  classes.num_categories = 1;

  bool enter_another = true;
  do {
    classes.categories[classes.num_categories - 1] = read_string("Category name: ");
    enter_another = read_boolean("Enter another? [y/n] ");
    if (enter_another) {
      classes.num_categories += 1;
      classes.categories = (my_string*)realloc(classes.categories, classes.num_categories*sizeof(my_string));
    } else {
      break;
    }
  } while(enter_another);

  return classes;
}

void print_classes(Classifier_List classes) {
  for (int i = 0; i < classes.num_categories; i++) {
    printf("Categories: %s\n", classes.categories[i].str);
  }
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

  Classifier_List classes = read_classes_user();
  print_classes(classes);


  Dataset kicks = read_dataset_user(classes.num_categories);
  print_dataset(&kicks);

  free(classes.categories);
  return 0;
}
