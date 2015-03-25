#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define WORKERS_MAX 40

#define STATION_NB_VALUES 2
#define STATION_MIN 10
#define STATION_STEP 10
#define OPERATION_NB_VALUES 2
#define OPERATION_MIN 60
#define OPERATION_STEP 20

#define PROC_TIME_NB_VALUES 100
#define PROC_TIME_MIN 1
#define PROC_TIME_STEP 1
#define WORKERS_LB_NB_VALUES 4
#define WORKERS_LB_MIN 1
#define WORKERS_LB_STEP 1
#define WORKERS_UB_MAX (WORKERS_LB_NB_VALUES * (WORKERS_LB_STEP - 1) + WORKERS_LB_MIN) * 2

#define PRECEDENCE_PROBABILITY 0.6


typedef struct Task Task;
typedef struct Station Station;
typedef struct Worker Worker;

float CreateNewInstance(Station*** stations, Task*** tasks, unsigned int seed);

void CommonHeuristicProcess(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int), int (*behindDueFunction)(Task**, float));
Worker** Heuristic(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int), int (*behindDueFunction)(Task**, float));
Worker** HeuristicCalculation(Station** stations, Task** tasks, Task** G, Task** Nplus, Worker** workers, int GSize, int NplusSize, int (*selectionFunction)(Task**, int));

int selection_topLong(Task** Nplus, int NplusSize);
int selection_topRandom(Task** Nplus, int NplusSize);
int behindDue_Common(Task** tasks, float Cmax);

int InitHeuristic(Task** tasks, Task*** G, Task*** Nplus, int* GSize, int* NplusSize, int first);
int UpdateGNplus(Task*** G, Task*** Nplus, int* GSize);

void WriteSolution(Worker** workers);
void ClearWorkers(Worker** workers);


#endif // MAIN_H_INCLUDED