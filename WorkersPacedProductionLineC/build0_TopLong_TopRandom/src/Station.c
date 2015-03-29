#include <stdlib.h>
#include "Station.h"

Station* CreateStation(int id)
{
    Station* newStation = (Station*)malloc(sizeof(Station));
    newStation->id = id;
    newStation->tasks = (Task**)malloc(0);

    newStation->nbTasks = 0;

    _nbStations++;

    return newStation;
}
void AddTask(Station* station, struct Task* task)
{
    station->tasks = (Task**)realloc(station->tasks, ++station->nbTasks * sizeof(Task*));
    station->tasks[station->nbTasks - 1] = task;
}

void DeleteStation(Station* station)
{
    free(station->tasks);
    free(station);

    _nbStations--;
}
