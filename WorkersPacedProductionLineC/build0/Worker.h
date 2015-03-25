#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

typedef struct Worker Worker;
typedef struct Task Task;

int _nbWorkers;

struct Worker
{
    int id;
    Task** doneTasks;
    float availabilityTime;

    int nbTasks;
};

Worker* CreateWorker(int id);
void ReinitWorker(Worker* worker);
void AssignTask(Worker* worker, Task* task);
void SortAsc(Worker** workers);
void DeleteWorker(Worker* worker);

#endif // WORKER_H_INCLUDED
