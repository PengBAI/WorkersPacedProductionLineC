#ifndef STATION_H_INCLUDED
#define STATION_H_INCLUDED

typedef struct Station Station;
typedef struct Task Task;

int _nbStations;

struct Station
{
    int id;
    Task** tasks;

    int nbTasks;
};

Station* CreateStation(int id);
void AddTask(Station* station, Task* task);
void DeleteStation(Station* station);

#endif // STATION_H_INCLUDED
