#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

typedef struct Worker Worker;
typedef struct Task Task;

#include "Utility.h"
int _nbWorkers;
int WORKERS_MAX;

struct Worker
{
    int id;
    Task** doneTasks;
    float availabilityTime;

    int nbTasks;

	int boolCriticalPath;
};

Worker* CreateWorker(int id);
void ReinitWorker(Worker* worker);
void ClearWorkers(Worker*** workers);
void AssignTask(Worker* worker, Task* task, int beginningTime);
float SortAsc(Worker** workers, int nbWorkersToAssign, float dateDispo, int * tableauWorkers);
int SetParallelTask(Worker** workers, int nbWorkersToAssign, float dateDispo, int beginningTime);
void DeleteWorker(Worker* worker);

#endif // WORKER_H_INCLUDED
