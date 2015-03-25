#include "Utility.h"

void** RemoveAt(void** array, int arraySize, int index)
{
    int i = 0;

    for (i = index; i < arraySize - 1; i++)
        array[i] = array[i + 1];

    return array;
}
int maxInt(int value1, int value2)
{
    return (int)maxFloat((float)value1, (float)value2);
}
float maxFloat(float value1, float value2)
{
    return value1 > value2 ? value1 : value2;
}
int minInt(int value1, int value2)
{
    return value1 < value2 ? value1 : value2;
}
