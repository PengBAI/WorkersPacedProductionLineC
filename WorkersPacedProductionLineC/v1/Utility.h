#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include "main.h"

#include "Task.h"
#include "Station.h"
#include "Worker.h"

#include <gtk\gtk.h>

#include <cairo.h>


void** RemoveAt(void** array, int arraySize, int index);
int maxInt(int value1, int value2);
float maxFloat(float value1, float value2);
int minInt(int value1, int value2);
float minFloat(float value1, float value2);
char * floatToChar(float numberFloat);

#endif  UTILITY_H_INCLUDED
