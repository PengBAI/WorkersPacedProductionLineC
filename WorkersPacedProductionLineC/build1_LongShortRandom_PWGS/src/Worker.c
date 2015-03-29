#include "Worker.h"

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
	worker->boolCriticalPath = 0;

    worker->nbTasks = 0;
}

void ClearWorkers(Worker*** workers)
{
    int i;
    int nbWorkers = _nbWorkers;
    for (i = 0; i < nbWorkers; i++)
    {
        DeleteWorker((*workers)[i]);
    }
    free(*workers);
}

void AssignTask(Worker* worker, Task* task, int beginningTime)
{
    worker->doneTasks = (Task**)realloc(worker->doneTasks, ++worker->nbTasks * sizeof(Task*));
    worker->doneTasks[worker->nbTasks - 1] = task;
    task->assignedWorkers = (Worker**)realloc(task->assignedWorkers, ++task->nbAssignedWorkers * sizeof(Worker*));
    task->assignedWorkers[task->nbAssignedWorkers - 1] = worker;
	
	if (worker->boolCriticalPath == 0)
	{
		if (worker->nbTasks > 1)
		{
			if ((int)(worker->doneTasks[worker->nbTasks - 2]->endingTime) != beginningTime)
				worker->boolCriticalPath = 1;
		}
	}
	
}

float SortAsc(Worker** workers, int nbWorkersToAssign, float dateDispo, int * tableauWorkers)
{
	int i;
	float min = LLONG_MAX;
	float newDateDispo = dateDispo;
	int position = 0;
	for (i = 0; i < nbWorkersToAssign; i++) {
		tableauWorkers[i] = -1;
	}

	for (i = 0; i < _nbWorkers; i++) {
		float availabilityTime = workers[i]->availabilityTime;
		if (availabilityTime <= dateDispo) {
			tableauWorkers[position] = i;
			position++;
			if (position == nbWorkersToAssign) break;
		}
		else if (availabilityTime < min) {
			min = availabilityTime;
		}
	}

	while (position != nbWorkersToAssign) {
		position = 0;
		newDateDispo = min;
		min = LLONG_MAX;
		for (i = 0; i < _nbWorkers; i++) {
			float availabilityTime = workers[i]->availabilityTime;
			if (availabilityTime <= newDateDispo) {
				tableauWorkers[position] = i;
				position++;
				if (position == nbWorkersToAssign) break;
			}
			else if (availabilityTime < min) {
				min = availabilityTime;
			}
		}
	}

	return newDateDispo;
}

int SetParallelTask(Worker** workers, int nbWorkersToAssign, float dateDispo, int beginningTime)
{
	int NumberWorkersAvailable=0;
	int i;
	if (dateDispo > beginningTime)
		return -1;
	else{
		for (i = 0; i < _nbWorkers; i++){
			float availabilityTime = workers[i]->availabilityTime;
			if (availabilityTime <= beginningTime)
				NumberWorkersAvailable++;
		}
		if (NumberWorkersAvailable < nbWorkersToAssign)
			return -1;
		else{
			return 0;
		}
	}
}

void DeleteWorker(Worker* worker)
{
    free(worker->doneTasks);
    free(worker);

    _nbWorkers--;
}
