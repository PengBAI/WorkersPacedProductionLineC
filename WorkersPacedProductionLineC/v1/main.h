#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "Utility.h"
// variables to create an instance

#define VALUE_MAX 100

#define STATION_NB_VALUES 2		//	Variables for
#define STATION_MIN 10			//	the number
#define STATION_STEP 10			//	of stations		ex: values: 2; min: 10; step: 10 -> 10 or 20
#define OPERATION_NB_VALUES 2		//	Variables for
#define OPERATION_MIN 60			//	the number
#define OPERATION_STEP 20			//	of tasks

#define PROC_TIME_NB_VALUES 100	//	Variables
#define PROC_TIME_MIN 1			//	to calculate
#define PROC_TIME_STEP 1		//	the processing times
#define WORKERS_LB_NB_VALUES 4		//	Variables for
#define WORKERS_LB_MIN 1			//	the minimum
#define WORKERS_LB_STEP 1			//	number of workers
#define WORKERS_UB_MAX ((WORKERS_LB_NB_VALUES) * ((WORKERS_LB_STEP) - 1) + (WORKERS_LB_MIN)) * 2	// Variable to define the max number of workers -- NOT USED

#define PRECEDENCE_PROBABILITY 0.6


typedef struct Task Task;
typedef struct Station Station;
typedef struct Worker Worker;

void Benchmark(int create);
void CreateNewInstance(int nInstance);			// create an instance from defined variables; stations: pointer to the array of stations, tasks: pointer to the array of tasks, seed: random seed to generate the data, fixedNbStations: if > 0 it's the number of stations to generate, fixedNbTasks: if > 0 it's the number of tasks to generate; return the due date
float LoadInstance(Station*** stations, Task*** tasks, char* fileName);															// load an existing instance; stations: pointer to the array of stations, tasks: pointer to the array of tasks, fileName: path to the file containing the instance; return the due date
void SaveInstance(Station** stations, Task** tasks, float timeBound, char* fileName);											// save an instance to a file; stations: array of stations, tasks: array of tasks, fileName: path to the file the instance will be saved
void clean(char *buffer, FILE *fp);
int CommonHeuristicProcess(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int, Worker**), int (*behindDueFunction)(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay), int gtk);//(Task**, float), int gtk);				// common process to calculate an heuristic and printing the results (parameters are described below)
Worker** Heuristic(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int, Worker**), int (*behindDueFunction)(Task**, int **, int));						// calculate an heuristic; stations: array of stations, tasks: array of tasks, timeBound: due date, selectionFunction: a function to find the task to process, behindDueFunction: a function to change tasks parameter when failing to find a solution; return an array of workers containing who is working where and when
Worker** HeuristicCalculation(Station** stations, Task** tasks, Task** G, Task** Nplus, Worker** workers, int GSize, int NplusSize, int (*selectionFunction)(Task**, int, Worker**));	// do not call directly - inner to Heuristic(...); stations: array of stations, tasks: array of tasks, G: array of tasks still needed to be processed, Nplus: array of tasks with no predecessors not done, selectionFunction: a function to find the task to process; return an array of workers containing who is working where and when

int selection_topLong(Task** Nplus, int NplusSize, Worker** workers);		// select the longest task; Nplus: array of tasks to investigate, NplusSize: number of elements in Nplus, workers: array of workers; return the index of the task to process (from Nplus)
int selection_topShort(Task** Nplus, int NplusSize, Worker** workers);
int selection_topRandom(Task** Nplus, int NplusSize, Worker** workers);	// select a random task; Nplus: array of tasks to investigate, NplusSize: number of elements in Nplus, workers: array of workers; return the index of the task to process (from Nplus)
int selection_test(Task** Nplus, int NplusSize, Worker** workers);
int behindDue_Common(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay);		// rearrange tasks to do a better job; tasks: array of all tasks, Cmax: time at which all tasks were complete; return the new number of workers
int behindDue_PWGS(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay);		// rearrange tasks to do a better job; tasks: array of all tasks, Cmax: time at which all tasks were complete; return the new number of workers
int InitHeuristic(Task** tasks, Task*** G, Task*** Nplus, int* GSize, int* NplusSize, int first);	// initialize common parameters to calculate an heuristic; tasks: array of tasks, G: pointer to the array of tasks to do, Nplus: pointer to the array of tasks which have no predecessors not done, GSize: number of elements in G, NplusSize: number of elements in Nplus, first: whether it is the first initialisation (so it doesn't erase changed variables such as the number of workers on a task); return the minimum number of workers needed
int InitGNplus(Task*** G, Task*** Nplus, int* GSize);	// update the 2 arrays G et Nplus when Nplus is empty; G: pointer to the array of tasks to do, Nplus: pointer to the array of tasks which have no predecessors not done, GSize: number of elements in G; return the number of elements in Nplus
int UpdateNplus(int NplusSize, Task*** Nplus, Task *** TasksDone, int nbTasksDone);// update the 2 arrays G et Nplus when Nplus is empty; G: pointer to the array of tasks to do, Nplus: pointer to the array of tasks which have no predecessors not done, GSize: number of elements in G; return the number of elements in Nplus
int UpdateGNplus(Task*** G, Task*** Nplus, Task *** SetTasks, int* GSize, int nbSetTasks);
void ViderG(Task*** G, int* GSize);


void WriteSolution(Worker** workers, char *selection);	// write the solution found on screen; workers: array of assigned workers


#endif  MAIN_H_INCLUDED