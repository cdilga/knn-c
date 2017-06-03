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

//Define the debug level. Outputs verbose output if enabled.
#define DEBUG

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

// typedef struct csv_file {
//   char data[][][255];
//   int num_line;
//   int num_fields;
// } CSV_File;

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
  printf("[DEBUG] Values[%d]: %d\n", 0, values[0]);
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
      printf("[DEBUG] Max index updated to %d\n", i - 1);
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
      printf("[DEBUG] Max index updated to %d\n", i - 1);
      #endif
    }

    //Keep a reference to an instance of the highest counted number in the array
    #ifdef DEBUG
    printf("[DEBUG] Values[%d]: %d\n", i, values[i]);
    #endif
  }

  return values[max_index];
}

//Doing a k nearest neighbour search

int knn_search(int k, Comparison_Point compare, Dataset *datapoints) {
  //Warn if k is even
  if (k % 2 == 0) {
    printf("[DEBUG] Warning: %d is even. Tie cases have undefined behviour\n", k);
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
    printf("[DEBUG] Point distance: %lf\n", distance);
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
        printf("[DEBUG] update_index: %d\nNeighbour Distance: %lf\n", j, compare.neighbour[j].distance);
        #endif
      }
    }

    //if the current point distance is less than the largest recorded distance, or if the distances haven't been set
    if (compare.neighbour[update_index].neighbour_pointer == NULL || compare.neighbour[update_index].distance > distance) {
      //Update the distance at update_index
      #ifdef DEBUG
      printf("[DEBUG] Neighbour number: %d is null, updating pointer\n", i);
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
    printf("[DEBUG] Category: %d\n", neighbour_categories[i]);
    #endif
  }

  //Find the mode of the categories
  //Call fuction with array of int and the length of the array and return the result
  //printf("mode of categories: %d\n", mode(neighbour_categories, k));

  //TODO fix this memory leak by doing non-dynamic allocation at some point
  return mode(neighbour_categories, k);
  //free(compare.neighbour);
}


//Function that takes in a classification integer, and returns a classification string
//Requires a map between the integers and the string in the form of a classification_map datatype
my_string classify(Classifier_List category_map, int category) {
  my_string class = category_map.categories[category];
  return class;
}

//Function that returns a classifier list read in from file
//Reads in the labels file, specified in parameters
//Generates a number of labels, inside of a classifier list and records the number
//of classifiers in the classifier list

Point read_point_user(int num_dimensions, int num_categories) {
  Point user_point;
  user_point.dimension = (float*)malloc(num_dimensions*sizeof(float));
  for (int i = 0; i < num_dimensions; i++) {
    printf("%dth dimension: ", i);
    user_point.dimension[i] = read_float("");
  }
  user_point.category = read_integer_range("Enter a category ID: ", 0, num_categories - 1);
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

Comparison_Point read_comparison_point_user(int num_dimensions) {
  Comparison_Point user_point;
  user_point.dimension = (float*)malloc(num_dimensions*sizeof(float));
  for (int i = 0; i < num_dimensions; i++) {
    printf("%dth dimension: ", i);
    user_point.dimension[i] = read_float("");
  }

  //TODO fix memory allocation
  return user_point;
}

int count_fields(char *buffer) {
  int count = 1;
  int pos = 0;
  char current;
  do {
    current = buffer[pos];
    // printf("%c", current);
    if (current == ',') {
      count++;
    }
    pos++;
  } while(current != '\n' && current != '\0');
  #ifdef DEBUG
  printf("[DEBUG] Number of fields: %d\n", count);
  #endif
  return count;
}

int get_class_num(my_string in_string, Classifier_List *class_list) {
  //Check to see if any of the strings are present in the classifier list
  //Could improve with a Levenshtein Distance calculation to account for human errors
  //Also, if i is zero, we won't even need to check ifit's in there, we know it's not
  #ifdef DEBUG
  printf("[DEBUG] class_list->num_categories: %d\n", class_list->num_categories);
  #endif

  for (int i = 0; i < class_list->num_categories; i++) {
    if (strcmp(class_list->categories[i].str, in_string.str) == 0) {
      return i;
    }
  }
  //If it isn't present in the existing array, we need to add it in.
  //Increment the count of categories
  class_list->num_categories++;
  #ifdef DEBUG
  printf("[DEBUG] Class list categories: %d\n", class_list->num_categories);
  #endif
  class_list->categories = realloc(class_list->categories, sizeof(my_string) * class_list->num_categories);
  class_list->categories[class_list->num_categories - 1] = in_string;
  return class_list->num_categories - 1;
}

//Function to read lines from CSV
//Takes a file name
my_string extract_field(my_string line, int field) {
  my_string return_value;
  //Using https://support.microsoft.com/en-us/help/51327/info-strtok-c-function----documentation-supplement
  if (field > count_fields(line.str)) {
    strcpy(return_value.str, "\0");
    return return_value;
  }
  //Potentially unsafe
  char *token;

  token = strtok(line.str, " ,");
  //Call strtok "field" times
  //Return that value of the token
  for (int i = 1; i < field; i++) {
    #ifdef DEBUG
    printf("[DEBUG] Token is:  %s\n", token);
    #endif

    token = strtok(NULL, " ,");
    #ifdef DEBUG
    printf("[DEBUG] Before copy in loop\n");
    #endif
  }
  strncpy(return_value.str, token, sizeof(return_value.str));

  return return_value;
}

int count_lines(my_string filename) {
  FILE *file;
  if (access(filename.str, F_OK) == -1) {
    printf("[ERROR] Could not find file");
    return -1;
  }
  file = fopen(filename.str, "r");
  char buffer[1024];
  int count = 0;
  while(fgets(buffer, 1024, file)) {
    count++;
  }
  fclose(file);
  #ifdef DEBUG
  printf("[DEBUG] Line number is:  %d\n", count);
  #endif
  return count;
}

Dataset new_dataset() {
  Point *points = {NULL};
  Dataset new = {0, 0, points};
  return new;
}

//function that takes in a line, and returns a point
//parse point
//TODO ADD UNIT TESTS!!!
Point parse_point(my_string line, int num_dimensions, Classifier_List *class_list) {
  float *dimensions = (float*)malloc(num_dimensions*sizeof(float));
  for (int i = 0; i < num_dimensions; i++) {
    //Go through and pull out the first num fields, and construct a point out of them
    // pass the string into a function that just mocks out and returns 1


    //Since the extract_field function extracts with a base 1, rather than base of 0
    dimensions[i] = atof(extract_field(line, i + 1).str);
  }

  Point curr_point;
  curr_point.dimension = dimensions;

  //Since the data for the class is one after the
  curr_point.category = get_class_num(extract_field(line, num_dimensions + 1), class_list);
  #ifdef DEBUG
  print_point(&curr_point, num_dimensions);
  #endif
  return curr_point;
}

Dataset read_dataset_file(my_string filename, Classifier_List *class_list) {
  // Read the number of lines in before opening the files
  int num_lines = count_lines(filename);

  //From that, it should return some struct
  FILE *file;
  if (access(filename.str, F_OK) == -1) {
    printf("[ERROR] Could not find file");
  }
  file = fopen(filename.str, "r");

  //Struct should contain a 2d array with the lines, in each with data separated into array elements
  char *buffer;
  buffer = (char*)malloc(sizeof(char) * 1024);
  fscanf(file, "%s\n", buffer);

  //Count the commas
  int num_dimensions = count_fields(buffer) - 1;

  //create a Dataset which can hold the rest of the data
  //dimensionality is the number of fields -1
  //number of points is the number of lines -1, assuming the last line is a blank line
  Point *points = (Point*)malloc((num_lines - 1)*sizeof(Point));
  Dataset return_dataset = {num_dimensions, num_lines - 1, points};

  my_string buffer_string;
  strcpy(buffer_string.str, buffer);

  int i = 0;
  //For each line, parse the point and add it to the dataset
  do {
    points[i] = parse_point(buffer_string, num_dimensions, class_list);

    i++;
    //Don't do this on the last iteration of the loop
    if (!(i == num_lines - 1)) {
      fscanf(file, "%s\n", buffer);
      strcpy(buffer_string.str, buffer);
    }
  } while (i < num_lines - 1);

  // Now we can essentially read in the first "count" fields and cast to float
  // Read in the last field, IE count and add a class for the
  free(buffer);
  return return_dataset;
}

Classifier_List new_classifier_list() {
  int num_categories = 0;
  my_string *categories;
  categories = malloc(sizeof(my_string));
  Classifier_List new_list = {categories, num_categories};
  return new_list;
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

  Classifier_List class_list = new_classifier_list();
  my_string filename = {"flowers.csv"};
  read_dataset_file(filename, &class_list);

  // Classifier_List classes = read_classes_user();
  // print_classes(classes);
  //
  //
  // Dataset kicks = read_dataset_user(classes.num_categories);
  // print_dataset(&kicks);
  //
  // Comparison_Point compare = read_comparison_point_user(kicks.dimensionality);
  //
  // printf("Classified as: %s", classify(classes, knn_search(3, compare, &kicks)).str);
  //
  // free(classes.categories);
  return 0;
}
