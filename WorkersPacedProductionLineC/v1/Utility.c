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
    return (int)minFloat((float)value1, (float)value2);
}
float minFloat(float value1, float value2)
{
    return value1 < value2 ? value1 : value2;
}

char * floatToChar(float numberFloat) {
	int partieDecimale2, partieDecimale1; 
	int partieEntiere = floor(numberFloat);
	int newPartieEntiere = partieEntiere;
	char * result;
	int puissance = 1;
	while (newPartieEntiere > 10) {
		newPartieEntiere = floor(newPartieEntiere / 10.0);
		puissance++;
	}
	result = (char *)malloc(sizeof(char)*(puissance + 3));
	partieDecimale1 = floor(numberFloat * 10) - (partieEntiere * 10);
	partieDecimale2 = floor(numberFloat * 100) - (partieDecimale1 * 10) - (partieEntiere * 100);

	sprintf(result, "%d,%d%d", partieEntiere, partieDecimale1, partieDecimale2);
	return result;
}