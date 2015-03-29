#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include "main.h"
#include "Utility.h"
#include "Task.h"
#include "Station.h"
#include "Worker.h"


int main()
{
    unsigned int randSeed = 1; //(int)time(NULL);
    Station** stations = NULL;
    Task** tasks = NULL;
    Worker** workers = NULL;
    float timeBound = 0;

    _nbStations = 0;
    _nbTasks = 0;
    _nbWorkers = 0;

    timeBound = CreateNewInstance(&stations, &tasks, randSeed);

    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_Common);
	CommonHeuristicProcess(stations, tasks, timeBound, selection_topRandom, behindDue_Common);

    return 0;
}


float CreateNewInstance(Station*** stations, Task*** tasks, unsigned int seed)
{

    int nbStations;
	int nbTasks;
	float A = 0; // to calculate the cycle time bound
	int i = 0;
	int j = 0;
	int k = 0;
	float timeBound;

    srand(seed);
	nbStations = (rand() % STATION_NB_VALUES) * STATION_STEP + STATION_MIN;
	nbTasks = (rand() % OPERATION_NB_VALUES) * OPERATION_STEP + OPERATION_MIN;

    // init stations
    free(*stations);
    *stations = (Station**)malloc(nbStations * sizeof(Station*));
    for (i = 0; i < nbStations; i++)
    {
        (*stations)[i] = CreateStation(i);
    }

    // init tasks
	free(*tasks);
	*tasks = (Task**)malloc(nbTasks * sizeof(Task*));
	for (i = 0; i < nbTasks; i++)
	{
		int processingTime = (rand() % PROC_TIME_NB_VALUES) * PROC_TIME_STEP + PROC_TIME_MIN;
		int workersLB = (rand() % WORKERS_LB_NB_VALUES) * WORKERS_LB_STEP + WORKERS_LB_MIN;
		int workersUB = workersLB * 2;
		(*tasks)[i] = CreateTask(i, (float)processingTime, (*stations)[rand() % nbStations], workersLB, workersUB);
		A += processingTime * (workersLB + workersUB) / 2;
	}

	// init time bound
	A /= nbStations;
	timeBound = rand() % (int)(0.5 * A) + A;

	// init precedences
	for (i = 0; i < nbStations; i++)
	{
		for (j = (*stations)[i]->nbTasks - 1; j >= 0; j--)
		{
			for (k = j - 1; k >= 0; k--)
			{
				if (rand() / RAND_MAX <= PRECEDENCE_PROBABILITY)
				{
				    AddPredecessor((*stations)[i]->tasks[j], (*stations)[i]->tasks[k]);
				}
			}
		}
	}

	return timeBound;
}

void CommonHeuristicProcess(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int), int (*behindDueFunction)(Task**, float))
{
	Worker** workers = NULL;
	struct timeb beginning;
	struct timeb ending;
	int processTime;
	
	ftime(&beginning);
	workers = Heuristic(stations, tasks, timeBound, (*selectionFunction), (*behindDueFunction));
	ftime(&ending);
	processTime = ending.millitm - beginning.millitm;

    WriteSolution(workers);
	printf("Processing Time: %dms\n", processTime);

    ClearWorkers(workers);
}
Worker** Heuristic(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int), int (*behindDueFunction)(Task**, float))
{
    int i;
    Task** G = NULL;
    Task** Nplus = NULL;
    Worker** workers;
    int GSize;
    int NplusSize;
    int nbWorkers;
	float Cmax;

    nbWorkers = InitHeuristic(tasks, &G, &Nplus, &GSize, &NplusSize, 1);
    workers = (Worker**)malloc(nbWorkers * sizeof(Worker*));
    for (i = 0; i < nbWorkers; i++)
        workers[i] = CreateWorker(i);

    Cmax = 0;
    do
    {

        workers = HeuristicCalculation(stations, tasks, G, Nplus, workers, GSize, NplusSize, (*selectionFunction));

		// calculate total processing
        Cmax = 0;
        for (i = 0; i < _nbWorkers; i++)
        {
            Cmax = maxFloat(Cmax, workers[i]->availabilityTime);
        }

		// if total processing greater than due date
        if (Cmax > timeBound)
        {
            nbWorkers = (*behindDueFunction)(tasks, Cmax);
            workers = (Worker**)realloc(workers, nbWorkers * sizeof(Worker*));
            for (i = 0; i < _nbWorkers; i++)
                ReinitWorker(workers[i]);
            for (i = _nbWorkers; i < nbWorkers; i++)
                workers[i] = CreateWorker(i);
            InitHeuristic(tasks, &G, &Nplus, &GSize, &NplusSize, 0); // TODO: refaire avec le pdf -> pas tout recalculer
        }
    }while(Cmax > timeBound && _nbWorkers <= WORKERS_MAX);

    free(G);
    free(Nplus);
    if (Cmax > timeBound)
    {
        return NULL;
    }

    return workers;
}
Worker** HeuristicCalculation(Station** stations, Task** tasks, Task** G, Task** Nplus, Worker** workers, int GSize, int NplusSize, int (*selectionFunction)(Task**, int))
{
    int i;
	int chosenTaskIndex;
	int nbWorkersToAssign;
    do
    {
        if (NplusSize == 0) // NplusSize = Nombre de taches éligibles
            NplusSize = UpdateGNplus(&G, &Nplus, &GSize);

        while (NplusSize > 0) // essayer en tri
        {
			j++;
            chosenTaskIndex = (*selectionFunction)(Nplus, NplusSize);
            nbWorkersToAssign = Nplus[chosenTaskIndex]->nbWorkersToAssign;

            SortAsc(workers);
            for (i = 0; i < nbWorkersToAssign; i++)
            {
                AssignTask(workers[i], Nplus[chosenTaskIndex]);
            }
            ExerciseTask(Nplus[chosenTaskIndex], workers[nbWorkersToAssign - 1]->availabilityTime);

            RemoveAt((void**)Nplus, NplusSize--, chosenTaskIndex);
        }
    }while (GSize > 0);

    return workers;
}

int selection_topLong(Task** Nplus, int NplusSize)
{
    int i = 0;
    int indexMax = 0;
    float maxValue = 0;
    float tmp = 0;
    for (i = 0; i < NplusSize; i++)
    {
        maxValue = maxFloat(CalculateCurrentDuration(Nplus[i]), maxValue);
        if  (tmp != maxValue)
        {
            tmp = maxValue;
            indexMax = i;
        }
    }
    return indexMax;
}
int selection_topRandom(Task** Nplus, int NplusSize)
{
	return rand() % NplusSize;
}
int behindDue_Common(Task** tasks, float Cmax)
{
    int i;
    for (i = _nbTasks - 1; i >= 0; i--)
    {
        if(Cmax == tasks[i]->endingTime)
        {
            Cmax = tasks[i]->beginningTime;
            tasks[i]->nbWorkersToAssign = minInt(tasks[i]->nbWorkersToAssign + 1, minInt(_nbWorkers + 1, tasks[i]->workersMax));
        }
    }
    return _nbWorkers + 1;
}

int InitHeuristic(Task** tasks, Task*** G, Task*** Nplus, int* GSize, int* NplusSize, int first)
{
    int i = 0;
    int minWorkers = 0;

    free(*G);
    free(*Nplus);
    *G = (Task**)malloc(0);
    *Nplus = (Task**)malloc(0);
    *NplusSize = 0;
    *GSize = 0;

    for (i = 0; i < _nbTasks; i++)
    {
        ReinitTask(tasks[i]);

        if (tasks[i]->nbPredecessors == 0)
        {
            *Nplus = (Task**)realloc(*Nplus, ++(*NplusSize) * sizeof(Task*));
            (*Nplus)[*NplusSize - 1] = tasks[i];
            tasks[i]->availabilityTime = 0;
        }
        else
        {
            *G = (Task**)realloc(*G, ++(*GSize) * sizeof(Task*));
            (*G)[*GSize - 1] = tasks[i];
        }

        if (first == 1)
        {
            tasks[i]->nbWorkersToAssign = tasks[i]->workersMin;
            minWorkers = maxInt(minWorkers, tasks[i]->nbWorkersToAssign);
        }
    }

    return minWorkers;
}
int UpdateGNplus(Task*** G, Task*** Nplus, int* GSize)
{
    int i = 0;
    int NplusSize = 0;
    for (i = 0; i < *GSize; i++)
    {
        if ((*G)[i]->nbPredecessors == (*G)[i]->nbPredecessorsDone)
        {
            *Nplus = (Task**)realloc(*Nplus, ++NplusSize * sizeof(Task*));
            (*Nplus)[NplusSize - 1] = (*G)[i];
            RemoveAt((void**)*G, (*GSize)--, i--);
        }
    }

    return NplusSize;
}

void WriteSolution(Worker** workers)
{
    if (workers == NULL)
    {
        printf("Infeasible Solution.\n");
    }
    else
    {
        int i;
        int j;
        for (i = 0; i < _nbWorkers; i++)
        {
            printf("Worker %d: \n", workers[i]->id);
            for (j = 0; j < workers[i]->nbTasks; j++)
            {
                printf("\t %d: %f -> %f", workers[i]->doneTasks[j]->id, workers[i]->doneTasks[j]->beginningTime, workers[i]->doneTasks[j]->endingTime);
                if (j + 1 < workers[i]->nbTasks)
                {
                    float spread = workers[i]->doneTasks[j + 1]->beginningTime - workers[i]->doneTasks[j]->endingTime;
                    if (spread > 0)
                    printf(" ... %f", spread);
                }
                printf("\n");
            }
        }
        printf("Found solution: %d workers\n", _nbWorkers);
    }
}
void ClearWorkers(Worker** workers)
{
    int i;
    int nbWorkers = _nbWorkers;
    for (i = 0; i < nbWorkers; i++)
    {
        DeleteWorker(workers[i]);
    }
    free(workers);
}
