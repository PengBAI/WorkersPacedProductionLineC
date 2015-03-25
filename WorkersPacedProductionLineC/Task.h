#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#include "Utility.h"

typedef struct Task Task;
typedef struct Station Station;
typedef struct Worker Worker;

int _nbTasks;

struct Task
{
    int id;
    float duration;
    Task** predecessors;
    Task** successors;
    Station* station;
    int workersMin;
    int workersMax;
    int nbWorkersToAssign;

    float availabilityTime;
    Worker** assignedWorkers;
    float beginningTime;
    float endingTime;
    int nbPredecessorsDone;
    int nbAssignedWorkers;

    int nbPredecessors;
    int nbSuccessors;
};

Task* CreateTask(int id, float duration, Station* station, int workersMin, int workersMax);
void ReinitTask(Task* task);
void ClearTasks(Task*** tasks);
float CalculateCurrentDuration(Task* task);
float CalculateDuration(Task* task, int nbWorkers);
void ExerciseTask(Task* task, float beginningTime);
void assignCriticakTaskPathWay(Task** tasks, int **TasksCriticalPathWayRecop, int nbWorkers);
void AddPredecessor(Task* task, Task* predecessor);
void AddSuccessor(Task* task, Task* successor);
void DeleteTask(Task* task);

#endif // TASK_H_INCLUDED
