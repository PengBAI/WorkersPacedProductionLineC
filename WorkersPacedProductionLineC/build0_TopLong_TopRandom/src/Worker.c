#include <stdlib.h>
#include <stdio.h>
#include "Worker.h"
#include "Task.h"

Worker* CreateWorker(int id)
{
    Worker* newWorker = (Worker*)malloc(sizeof(Worker));
    newWorker->id = id;
    newWorker->doneTasks = NULL;
    ReinitWorker(newWorker);
	
    _nbWorkers++;

    return newWorker;
}

void ReinitWorker(Worker* worker)
{
    free(worker->doneTasks);
    worker->doneTasks = (Task**)malloc(0);
    worker->availabilityTime = 0;

    worker->nbTasks = 0;
}

void AssignTask(Worker* worker, Task* task)
{
    worker->doneTasks = (Task**)realloc(worker->doneTasks, ++worker->nbTasks * sizeof(Task*));
    worker->doneTasks[worker->nbTasks - 1] = task;
    task->assignedWorkers = (Worker**)realloc(task->assignedWorkers, ++task->nbAssignedWorkers * sizeof(Worker*));
    task->assignedWorkers[task->nbAssignedWorkers - 1] = worker;
}

void SortAsc(Worker** workers)
{
    int i;
    int j;
    Worker* tmp;
    for (i = 0; i < _nbWorkers - 1; i++)
    {
        for (j = i + 1; j<_nbWorkers; j++)
        {
            if (workers[i]->availabilityTime > workers[j]->availabilityTime)
            {
                tmp = workers[i];
                workers[i] = workers[j];
                workers[j] = tmp;
            }
        }
    }
}

void DeleteWorker(Worker* worker)
{
    free(worker->doneTasks);
    free(worker);

    _nbWorkers--;
}
