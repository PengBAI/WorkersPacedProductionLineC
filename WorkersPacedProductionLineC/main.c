#include "main.h"

#define WINDOW_WIDTH  300
#define WINDOW_HEIGHT 300

int main(int argc, char ** argv)
{
    int turn = 0;
    while(++turn){
        unsigned int randSeed = 1; //(int)time(NULL);
        Station** stations = NULL;
        Task** tasks = NULL;
        Worker** workers = NULL;
        float timeBound = 0;
        char chaine[50] = "";
        int heuristique = 0;
        int create = 0;
        int nombre = 0;
        char choix[50] = "";
        int solution = 0;
        int processTime = 0;

        gtk_init(&argc, &argv);

        printf("\n======= Choix %d =======\n\n", turn);
        printf("1. Vos propres donnees\n");
        printf("2. Benchmark\n");
        fgets(choix, sizeof(choix), stdin);
        clean(choix, stdin);
        sscanf(choix, "%d", &nombre);

        if (nombre == 1 || nombre == 2){
            _nbStations = 0;
            _nbTasks = 0;
            _nbWorkers = 0;
            WORKERS_MAX = 100;
            if (nombre == 1){
                printf("======= Fichier =======\n\n");
                printf("Veuillez donner le chemin d'acces de votre instance :\n");
                fgets(chaine, sizeof(chaine), stdin);
                clean(chaine, stdin);

                printf("======= Heuristique =======\n\n");
                printf("1. Top Long\n");
                printf("2. Top Short\n");
                printf("3. PWGS (Long)\n");
                printf("4. PWGS (Short)\n");
                printf("5. PWGSv2 (Long)\n");
                printf("6. PWGSv2 (Short)\n");
                printf("7. Top Random\n");
                printf("\nVotre choix ? ");
                scanf("%d", &heuristique);

                timeBound = LoadInstance(&stations, &tasks, chaine);

                switch (heuristique)
                {
                case 1:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_Common, 1, &processTime);
                    break;
                case 2:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_Common, 1, &processTime);
                    break;
                case 3:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGS, 1,&processTime);
                    break;
                case 4:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGS, 1,&processTime);
                    break;
                case 5:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGSv2, 1,&processTime);
                    break;
                case 6:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGSv2, 1,&processTime);
                    break;
                case 7:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topRandom, behindDue_Common, 1,&processTime);
                    break;
                default:
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_Common, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_Common, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGS, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGS, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGSv2, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGSv2, 1,&processTime);
                    CommonHeuristicProcess(stations, tasks, timeBound, selection_topRandom, behindDue_Common, 1,&processTime);
                }
                getchar();
                //return 0;
            }
            else{
                //Création de nouvelles instances aléatoires

                //Benchmark
                printf("=== Benchmark ===\n\n");
                printf("0. Garder les jeux de donnees tests\n");
                printf("1. Recrer un nouveau jeu de donnees\n");
                scanf("%d", &create);

                Benchmark(create);
                getchar();
                //return 0;
            }
        }
        else{
            printf("Erreur dans la sélection.");
            getchar();
            //return 0;
        }
    }
    return 0;
}

/*
 * test with the benchmark and write the result into file .csv
 */
void Benchmark(int create)
{
    Station** stations = NULL;
    Task** tasks = NULL;
    Worker** workers = NULL;
    FILE *fichier = NULL;
    char path[256];
    char chaine[256];
    int nbTasks[2] = { 60, 80 };
    int nbMaxWorker[3] = { 10, 30, 40 };
    int nbStations[2] = { 10, 20 };
    int prob[3] = { 3, 6, 9 };
    float timeBound = 0;
    int bestLong = 0, bestShort = 0, bestPLong = 0, bestPShort = 0, bestRand = 0, bestPShort2 = 0, bestPLong2 = 0;
    float averageLong = 0.0, averageShort = 0.0, averagePLong = 0.0, averagePShort = 0.0, averageRand = 0.0, averagePLong2 = 0.0, averagePShort2 = 0.0;
    int infaisableLong = 0, infaisableShort = 0, infaisablePLong = 0, infaisablePShort = 0, infaisableRand = 0, infaisablePLong2 = 0, infaisablePShort2 = 0;
    // process time
    int processTimeLong = 0, processTimeShort = 0, processTimeLongPWGS = 0, processTimeShortPWGS = 0, processTimeLongPWGSv2 = 0, processTimeShortPWGSv2 = 0;
    int tabMin[6];  // save min worker 
    int tabProcessTime[6] = {0,0,0,0,0,0}; // processTime
    int iPT = 0; // index for tabProcessTime[]
    int min = VALUE_MAX;
    int indexMin = VALUE_MAX;
    int i,j,k,z,a,b;
    sprintf(path, "../benchmarks/benchmark.csv");
    fichier = fopen(path, "w");

    fprintf(fichier, "n;rmax;s;prob;;TopLong;;;;TopShort;;;;PWGS(Long);;;;PWGS(Short);;;;PWGSv2(Long);;;;PWGSv2(Short);;;\n");
    fprintf(fichier, ";;;;Moyenne;ProcessTime;Infaisable;Best;Moyenne;ProcessTime;Infaisable;Best;Moyenne;ProcessTime;Infaisable;Best;Moyenne;ProcessTime;Infaisable;Best;Moyenne;ProcessTime;Infaisable;Best;Moyenne;ProcessTime;Infaisable;Best;\n");
    for (i = 0; i < 2; i++){
        for ( j = 0; j < 3; j++){
            for ( k = 0; k < 2; k++){
                for ( z = 0; z < 3; z++){
                    // test 10 instances
                    for ( a = 0; a < 10; a++){
                        if (create == 1){
                            CreateNewInstance(a);
                        }
                        sprintf(chaine, "../benchmarks/instance%d-%d-%d-%d-%d.txt", nbTasks[i], nbMaxWorker[j], nbStations[k], prob[z],a);
                        WORKERS_MAX = nbMaxWorker[j];
                        min = VALUE_MAX;
                        timeBound = LoadInstance(&stations, &tasks, chaine);
                        tabMin[0] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_Common, 0, &processTimeLong);

                        tabMin[1] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_Common, 0, &processTimeShort);

                        tabMin[2] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGS, 0, &processTimeLongPWGS);

                        tabMin[3] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGS, 0, &processTimeShortPWGS);

                        tabMin[4] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topLong, behindDue_PWGSv2, 0, &processTimeLongPWGSv2);

                        tabMin[5] = CommonHeuristicProcess(stations, tasks, timeBound, selection_topShort, behindDue_PWGSv2, 0, &processTimeShortPWGSv2);

                        for ( b = 0; b<6; b++){
                            if (tabMin[b] < min && tabMin[b] != 0){
                                min = tabMin[b];
                            }
                        }

                        if (tabMin[0] == min){
                            bestLong++;
                        }
                        if (tabMin[1] == min){
                            bestShort++;
                        }
                        if (tabMin[2] == min){
                            bestPLong++;
                        }
                        if (tabMin[3] == min){
                            bestPShort++;
                        }
                        if (tabMin[4] == min){
                            bestPLong2++;
                        }
                        if (tabMin[5] == min){
                            bestPShort2++;
                        }
                        averageLong = averageLong + (float)tabMin[0];
                        averageShort = averageShort + (float)tabMin[1];
                        averagePLong = averagePLong + (float)tabMin[2];
                        averagePShort = averagePShort + (float)tabMin[3];
                        averagePLong2 = averagePLong2 + (float)tabMin[4];
                        averagePShort2 = averagePShort2 + (float)tabMin[5];
                        /*averageRand = averageRand + solutionRand;*/

                        tabProcessTime[0] += processTimeLong; tabProcessTime[1] += processTimeShort;
                        tabProcessTime[2] += processTimeLongPWGS; tabProcessTime[3] += processTimeShortPWGS; 
                        tabProcessTime[4] += processTimeLongPWGSv2; tabProcessTime[5] += processTimeShortPWGSv2; 

                        if (tabMin[0] == 0){
                            infaisableLong++;
                        }
                        if (tabMin[1] == 0){
                            infaisableShort++;
                        }
                        if (tabMin[2] == 0){
                            infaisablePLong++;
                        }
                        if (tabMin[3] == 0){
                            infaisablePShort++;
                        }
                        if (tabMin[4] == 0){
                            infaisablePLong2++;
                        }
                        if (tabMin[5] == 0){
                            infaisablePShort2++;
                        }
                        /*if (solutionRand == 0){
                        infaisableRand++;
                        }*/
                    }
                    
                    // compute process average time
                    for(iPT = 0; iPT < 6; iPT++){
                        tabProcessTime[iPT] = tabProcessTime[iPT] / 6;
                    }

                    if (infaisableLong != 10)
                        averageLong = averageLong / (float)(10 - infaisableLong);
                    else
                        averageLong = 0.0;
                    if (infaisableShort != 10)
                        averageShort = averageShort / (float)(10 - infaisableShort);
                    else
                        averageShort = 0.0;
                    if (infaisablePLong != 10)
                        averagePLong = averagePLong / (float)(10 - infaisablePLong);
                    else
                        averagePLong = 0.0;
                    if (infaisablePShort != 10)
                        averagePShort = averagePShort / (float)(10 - infaisablePShort);
                    else
                        averagePShort = 0.0;
                    if (infaisablePLong2 != 10)
                        averagePLong2 = averagePLong2 / (float)(10 - infaisablePLong2);
                    else
                        averagePLong2 = 0.0;
                    if (infaisablePShort2 != 10)
                        averagePShort2 = averagePShort2 / (float)(10 - infaisablePShort2);
                    else
                        averagePShort2 = 0.0;
                    /*averageRand = averageRand /10;*/

                    fprintf(fichier, "%d;%d;%d;0,%d;", nbTasks[i], nbMaxWorker[j], nbStations[k], prob[z]);
                    fprintf(fichier, "%f;%d;%d;%d;%f;%d;%d;%d;%f;%d;%d;%d;%f;%d;%d;%d;%f;%d;%d;%d;%f;%d;%d;%d",
                        averageLong,
                        tabProcessTime[0],
                        infaisableLong,
                        bestLong,						
                        averageShort,
                        tabProcessTime[1],
                        infaisableShort,
                        bestShort,
                        averagePLong,
                        tabProcessTime[2],
                        infaisablePLong,
                        bestPLong,
                        averagePShort,
                        tabProcessTime[3],
                        infaisablePShort,
                        bestPShort,
                        averagePLong2,
                        tabProcessTime[4],
                        infaisablePLong2,
                        bestPLong2,
                        averagePShort2,
                        tabProcessTime[5],
                        infaisablePShort2,
                        bestPShort2
                        /*averageRand,
                        infaisableRand,
                        bestRand*/);
                    fprintf(fichier, "\n");
                    bestLong = 0; bestShort = 0; bestPLong = 0; bestPShort = 0; bestRand = 0; 
                    bestPLong2 = 0; bestPShort2 = 0;
                    averageLong = 0.0; averageShort = 0.0; averagePLong = 0.0; averagePShort = 0.0; averageRand = 0.0;
                    averagePLong2 = 0.0; averagePShort2 = 0.0;
                    infaisableLong = 0; infaisableShort = 0; infaisablePLong = 0; infaisablePShort = 0; infaisableRand = 0;
                    infaisablePLong2 = 0; infaisablePShort2 = 0;
                    for(iPT = 0; iPT < 6; iPT++){
                        tabProcessTime[iPT] = 0;
                    }
                }
            }
        }
    }
    fclose(fichier);
}

void CreateNewInstance(int nInstance)
{
    // Init
    int durationTask = 0;
    int numeroStation = 0;
    int numeroAi = 0;
    int numeroBi = 0;
    int numeroA = 0;
    int proba;
    int ** stations = (int **)malloc(0);
    int * tailleStations = (int *)malloc(0);
    int ** predecesseur = (int **)malloc(0);
    int nbTasks[2] = { 60, 80 };
    int nbMaxWorker[3] = { 10, 30, 40};
    int nbStations[2] = { 10, 20 };

    int nombreTaches= 2;
    int nombreRMAx = 3;
    int nombreStations = 2;

    int prob[3] = { 3, 6, 9 };
    int ai[4] = { 1, 2, 3, 4 };
    int i, j, k, l, z, m,y,w, iBoucle;
    char path[256];
    FILE *fichier = NULL;
    srand(time(NULL));

    // ecriture
    for (i = 0; i < nombreTaches; i++)
    {
        for (j = 0; j < nombreRMAx; j++)
        {
            for (k = 0; k < nombreStations; k++)
            {
                for (z = 0; z < 3; z++)
                {
                    sprintf(path, "../benchmarks/instance%d-%d-%d-%d-%d.txt", nbTasks[i], nbMaxWorker[j], nbStations[k], prob[z], nInstance);
                    fichier = fopen(path,"w");
                    if (fichier == NULL)
                        printf("Erreur a l'ouverture du fichier\n");
                    else {
                        // écriture dans le fichier
                        fprintf(fichier, "%d\t%d\n", nbStations[k], nbTasks[i]);
                        stations = (int**)realloc(stations, sizeof(int*)*nbStations[k]);
                        if (stations == NULL)
                        {
                            printf("Can't reallocate memory\n");
                        }
                        tailleStations = (int*)realloc(tailleStations, sizeof(int)*nbStations[k]);
                        if (tailleStations == NULL)
                        {
                            printf("Can't reallocate memory\n");
                        }
                        predecesseur = (int**)realloc(predecesseur, sizeof(int*)*nbTasks[i]);
                        if (predecesseur == NULL)
                        {
                            printf("Can't reallocate memory\n");
                        }

                        for (m = 0; m < nbStations[k]; m++){
                            stations[m] = (int*)malloc(0);
                            tailleStations[m] = 0;
                        }
                        /** On construit une matrice predecesseur pour voir **/
                        for (m = 0; m < nbTasks[i]; m++){
                            predecesseur[m] = (int*)malloc(sizeof(int)* 1);
                            predecesseur[m][0] = 0;
                        }
                        for (l = 0; l < nbTasks[i]; l++){
                            durationTask = rand() % 100 + 1;
                            numeroStation = rand() % nbStations[k];
                            tailleStations[numeroStation]++;
                            stations[numeroStation] = (int*)realloc(stations[numeroStation], sizeof(int)*tailleStations[numeroStation]);
                            stations[numeroStation][tailleStations[numeroStation] - 1] = l;

                            if (tailleStations[numeroStation] > 1){
                                for (y = (tailleStations[numeroStation]-1); y > 0; y--){
                                    proba = rand() % 10;
                                    if (proba < prob[z]){
                                        predecesseur[l][0]++;
                                        predecesseur[l] = (int*)realloc(predecesseur[l], sizeof(int)*(predecesseur[l][0]+1));
                                        predecesseur[l][predecesseur[l][0]] = stations[numeroStation][y-1];
                                    }
                                }
                            }

                            numeroAi = ai[rand() % 4];
                            numeroBi = rand() % (numeroAi + 1) + numeroAi;
                            numeroA = numeroA + ((durationTask*((numeroAi + numeroBi) / 2)));
                            fprintf(fichier, "%d\t%d\t%d\t%d\n", durationTask, numeroStation, numeroAi, numeroBi);
                        }
                        numeroA = (numeroA / nbStations[k])*1,5;
                        fprintf(fichier, "%d\n", numeroA);

                        for (y = 0; y < nbTasks[i]; y++){
                            for (w = 0; w <= predecesseur[y][0]; w++){
                                fprintf(fichier, "%d\t", predecesseur[y][w]);
                            }
                            fprintf(fichier, "\n");
                        }

                        fclose(fichier);
                    }
                }
            }
        }
        //free
        for (iBoucle = 0; iBoucle < nombreStations; iBoucle++){
            free(stations[iBoucle]);
        }
    }

    free(tailleStations);
    free(stations);
    for (i = 0; i < nombreTaches; i++)
        free(predecesseur[i]);
    free(predecesseur);
}

float LoadInstance(Station*** stations, Task*** tasks, char* fileName)
{
    int i, j;
    int nbStations, nbTasks;
    float timeBound;
    int nbPredecessors;
    FILE* file = fopen(fileName, "r");
    if (!file)
    {
        printf("Echec du chargement de l'instance dans le fichier %s.\n", fileName);
        return -1;
    }

    // load number of stations and tasks
    fscanf(file, "%d\t%d", &nbStations, &nbTasks);

    // create stations
    ClearStations(stations);
    *stations = (Station**)malloc(nbStations * sizeof(Station*));
    for (i = 0; i < nbStations; i++)
    {
        (*stations)[i] = CreateStation(i);
    }

    // create tasks
    ClearTasks(tasks);
    *tasks = (Task**)malloc(nbTasks * sizeof(Task*));
    for (i = 0; i < nbTasks; i++)
    {
        float processingTime;
        int stationId, workersLB, workersUB;
        fscanf(file, "%f\t%d\t%d\t%d\n", &processingTime, &stationId, &workersLB, &workersUB); // load tasks' info
        (*tasks)[i] = CreateTask(i, processingTime, (*stations)[stationId], workersLB, workersUB);
    }

    // load timeBound
    fscanf(file, "%f\n", &timeBound);

    // load and apply precedences
    for (i = 0; i < nbTasks; i++)
    {

        fscanf(file, "%d\t", &nbPredecessors);
        for (j = 0; j < nbPredecessors; j++)
        {
            int predecessorId;
            fscanf(file, "%d\t", &predecessorId);
            AddPredecessor((*tasks)[i], (*tasks)[predecessorId]);
        }
        fscanf(file, "\n");
    }

    fclose(file);

    return timeBound;
}

void SaveInstance(Station** stations, Task** tasks, float timeBound, char* fileName)
{
    int i, j;
    FILE* file;
    if (!fopen(fileName, "w+"))
    {
        printf("Echec de la sauvegarde dans le fichier %s.\n", fileName);
        return;
    }

    // write number of stations and tasks
    fprintf(file, "%d\t%d\n", _nbStations, _nbTasks);

    // write duration, station id, workers LB and UB for each tasks
    for (i = 0; i < _nbTasks; i++)
    {
        fprintf(file, "%f\t%d\t%d\t%d\n", tasks[i]->duration, tasks[i]->station->id, tasks[i]->workersMin, tasks[i]->workersMax);
    }

    // write timeBound
    fprintf(file, "%f\n", timeBound);

    // write precedences
    for (i = 0; i < _nbTasks; i++)
    {
        fprintf(file, "%d\t", tasks[i]->nbPredecessors);
        for (j = 0; j < tasks[i]->nbPredecessors; j++)
        {
            fprintf(file, "%d\t", tasks[i]->predecessors[j]->id);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

/*
 * Interface GUI draw guantt
 */
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, Worker *** workers)
{
    int cmax = 0;
    int FinTache, i, j;
    int cmaxRecop;
    int somme;
    char ChaineEchelle[15];
    int echelle = 0;
    int tailleLongueur;

    /* Set color for background */
    cairo_set_source_rgb(cr, 1, 1, 1);
    /* fill in the background color*/

    cairo_paint(cr);

    /** Initialisation **/

    tailleLongueur = (_nbWorkers + 1) * 50;
    //Ligne verticale
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1);
    cairo_move_to(cr, 50, tailleLongueur);
    cairo_line_to(cr, 50, 10);
    cairo_stroke(cr);

    //Ligne horizontal
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1);
    //50=decalage gauche
    cairo_move_to(cr, 50, tailleLongueur);


    //500= longueur horizontal, 400 =longueur vertical
    cairo_line_to(cr, 500, tailleLongueur);
    cairo_stroke(cr);

    for (i = 0; i < _nbWorkers; i++)
    {
        char worker[7] = "Worker";
        char test[5];
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 1);
        cairo_move_to(cr, 50, tailleLongueur - ((i + 1) * 50));
        cairo_line_to(cr, 40, tailleLongueur - ((i + 1) * 50));
        cairo_stroke(cr);

        cairo_move_to(cr, 0, tailleLongueur - ((i + 1) * 50));

        cairo_show_text(cr, worker);
        cairo_stroke(cr);

        cairo_move_to(cr, 35, tailleLongueur - ((i + 1) * 50));

        sprintf(test, "%d", i);
        cairo_show_text(cr, test);
        cairo_stroke(cr);

    }


    /**Dessin des taches des workers**/
    //calcul du CMax

    for ( i = 0; i < _nbWorkers; i++)
    {
        for ( j = 0; j < (*workers)[i]->nbTasks; j++)
        {
            FinTache = (*workers)[i]->doneTasks[j]->endingTime;

            if (FinTache > cmax)
                cmax = FinTache;
        }
    }

    //Generation de l'échelle
    cmaxRecop = cmax;
    if (cmaxRecop >= 100)
    {	
        echelle = 5;
        while (cmaxRecop > 500)
        {
            echelle += 5;
            cmaxRecop = cmaxRecop - 500;
        }
    }
    else
        echelle = 1;


    for (i = 0; i < _nbWorkers; i++)
    {
        for (j = 0; j < (*workers)[i]->nbTasks; j++)
        {
            float DebutTache = (*workers)[i]->doneTasks[j]->beginningTime;
            float FinTache = (*workers)[i]->doneTasks[j]->endingTime;
            float duration = CalculateCurrentDuration((*workers)[i]->doneTasks[j]);
            int numeroTache = (*workers)[i]->doneTasks[j]->id;
            float decBas = tailleLongueur - 50 * (i + 1);
            char numTache[5];
            float decGauche;
            float longH;
            char * result;
            char * result2;
            if (echelle == 1)
            {
                longH = duration * 10;
                decGauche = 50 + 10*DebutTache;
            }
            if (echelle == 5)
            {
                longH = duration*echelle;
                decGauche = 50 + echelle*DebutTache;
            }
            if (echelle == 10)
            {
                longH = duration;
                decGauche = 50 + DebutTache;
            }
            if (echelle > 10)
            {
                longH = duration *(1 / (echelle / 10));
                decGauche = 50 + (1 / (echelle / 10))*DebutTache;
            }

            // Création du rectangle
            cairo_rectangle(cr, decGauche, decBas, longH, 50);
            cairo_stroke(cr);

            //ecriture dans le rectangle
            cairo_move_to(cr, decGauche + longH/2  , tailleLongueur - 50 *i-25);
            cairo_set_font_size(cr, 8);

            sprintf(numTache, "%d", numeroTache);
            cairo_show_text(cr, numTache);
            cairo_stroke(cr);

            //écriture des indices

            //indice gauche
            if (floor(DebutTache) != DebutTache)
            {
                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_set_line_width(cr, 1);
                cairo_move_to(cr, decGauche, tailleLongueur);
                cairo_line_to(cr, decGauche, tailleLongueur + 30);
                cairo_stroke(cr);

                cairo_move_to(cr, decGauche-5, tailleLongueur + 40);

                cairo_set_font_size(cr, 7);
                result = floatToChar(DebutTache);
                cairo_show_text(cr, result);
                cairo_stroke(cr);
            }

            //indice droit
            if (floor(FinTache) != FinTache)
            {
                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_set_line_width(cr, 1);
                cairo_move_to(cr, (decGauche + longH), tailleLongueur);
                cairo_line_to(cr, (decGauche + longH), tailleLongueur + 30);
                cairo_stroke(cr);

                cairo_move_to(cr, (decGauche + longH -5), tailleLongueur + 40);

                cairo_set_font_size(cr, 7);

                result2 = floatToChar(FinTache);
                cairo_show_text(cr, result2);
                cairo_stroke(cr);
            }
        }
    }

    somme = echelle;
    //écriture des indices
    for (i = 0; i <=cmax; i++)
    {
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 1);
        cairo_move_to(cr, i * 10 + 60, tailleLongueur);
        cairo_line_to(cr, i * 10 + 60, tailleLongueur+10);
        cairo_stroke(cr);

        //ecriture sous le tiret
        cairo_move_to(cr, i * 10 +55, tailleLongueur + 20);

        cairo_set_font_size(cr, 6);

        sprintf(ChaineEchelle, "%d", echelle);
        cairo_show_text(cr, ChaineEchelle);
        cairo_stroke(cr);
        echelle += somme;

    }

    return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                        HEURISTIC START
//
//////////////////////////////////////////////////////////////////////////////////////////////////
int CommonHeuristicProcess(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int, Worker**), int (*behindDueFunction)(Task**, int **, int, float),int gtk, int* processTime)
{
    Worker** workers = NULL;
    struct timeb beginning;
    struct timeb ending;
    int nbWorkers;
    // try to calculate the time needed to do an heuristic
    ftime(&beginning);
    workers = Heuristic(stations, tasks, timeBound, (*selectionFunction), (*behindDueFunction));
    ftime(&ending);
    *processTime = ending.millitm - beginning.millitm;
    // writings to screen
    if (*selectionFunction == selection_topLong){
        WriteSolution(workers, "topLong");
    }
    else if (*selectionFunction == selection_topShort){
        WriteSolution(workers, "topShort");
    }
    else if (*selectionFunction == selection_topRandom){
        WriteSolution(workers, "topRandom");
    }
    
    printf("Processing Time: %d ms\n", *processTime);

    //GTK
    if (gtk == 1)
    {
        GtkWidget *fenetre;
        GtkWidget *dessin;

        fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(fenetre), 800, 500);
        gtk_window_set_title(GTK_WINDOW(fenetre), "Organisation");
        g_signal_connect(fenetre, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        dessin = gtk_drawing_area_new();
        gtk_widget_set_size_request(dessin, WINDOW_WIDTH, WINDOW_HEIGHT);

        g_signal_connect(dessin, "draw", G_CALLBACK(draw_cb), &workers);
        gtk_container_add(GTK_CONTAINER(fenetre), dessin);

        gtk_widget_show_all(fenetre);
        gtk_main();

    }
    nbWorkers = _nbWorkers;
    // free allocated memory
    ClearWorkers(&workers);
    return nbWorkers;
}

Worker** Heuristic(Station** stations, Task** tasks, float timeBound, int (*selectionFunction)(Task**, int, Worker**), int (*behindDueFunction)(Task**, int **, int, float))
{
    int i,k, j, trouve, idTask;
    int PassageWorker=0;
    int nbTasksCriticalPathWay;
    Task** G = NULL; // contain the tasks to process
    Task** Nplus = NULL; // contain the tasks we can actually process
    Worker** workers;
    int GSize; // number of elements in G
    int NplusSize; // number of elements in Nplus
    int nbWorkers;
    int iBoucle;
    float Cmax; // the total processing given by the heuristic
    int * tableauRJ = (int *)malloc(_nbTasks*sizeof(int));
    int * TasksCriticalPathWayRecop = (int *)malloc(0);
    int * TasksCriticalPathWay = (int *)malloc(0);

    // initialise workers
    nbWorkers = InitHeuristic(tasks, &G, &Nplus, &GSize, &NplusSize, 1);
    workers = (Worker**)malloc(nbWorkers * sizeof(Worker*));
    for (i = 0; i < nbWorkers; i++)
        workers[i] = CreateWorker(i);

    do
    {
        // calculate the heuristic
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
            nbTasksCriticalPathWay = 0;
            for (i = 0; i<nbWorkers; i++)
            {
                if (workers[i]->boolCriticalPath == 0)
                {
                    for (k = 0; k < workers[i]->nbTasks; k++)
                    {

                        j = 0;
                        trouve = 0;
                        idTask = workers[i]->doneTasks[k]->id;
                        while (j < nbTasksCriticalPathWay && trouve == 0)
                        {
                            if (TasksCriticalPathWay[j] == idTask)
                                trouve = 1;
                            j++;
                        }
                        if (trouve == 0)
                        {
                            nbTasksCriticalPathWay++;
                            TasksCriticalPathWay = (int *)realloc(TasksCriticalPathWay, sizeof(int)*nbTasksCriticalPathWay);
                            TasksCriticalPathWay[nbTasksCriticalPathWay - 1] = idTask;
                        }
                    }
                }
            }
            if (PassageWorker == 0)
            {
                TasksCriticalPathWayRecop = (int *)realloc(TasksCriticalPathWayRecop, sizeof(int)*(nbTasksCriticalPathWay + 1));
                //Garder RJ dans un tableau
                for (iBoucle = 0; iBoucle < _nbTasks; iBoucle++)
                    tableauRJ[iBoucle] = tasks[iBoucle]->nbAssignedWorkers;

                //on enregistre chemin critique + rj
                for (iBoucle = 0; iBoucle < nbTasksCriticalPathWay; iBoucle++)
                    TasksCriticalPathWayRecop[iBoucle] = TasksCriticalPathWay[iBoucle];

                TasksCriticalPathWayRecop[nbTasksCriticalPathWay] = -1;
                PassageWorker = 1;


            }
            // ordonnance les workers
            nbWorkers = (*behindDueFunction)(tasks, &TasksCriticalPathWay, nbTasksCriticalPathWay, Cmax - timeBound);

            if (_nbWorkers != nbWorkers)
            {
                PassageWorker = 0;

                for (iBoucle = 0; iBoucle < _nbTasks; iBoucle++)
                {
                    tasks[iBoucle]->nbWorkersToAssign = tableauRJ[iBoucle];
                }
                //Mise à jour rj+1 dans taches du chemin critique
                //void assignCriticakTaskPathWay(Task** tasks, int **TasksCriticalPathWayRecop)
                assignCriticakTaskPathWay(tasks, &TasksCriticalPathWayRecop, nbWorkers);

            }
            if (nbWorkers > WORKERS_MAX)
                break;
            workers = (Worker**)realloc(workers, nbWorkers * sizeof(Worker*));
            for (i = 0; i < _nbWorkers; i++)
                ReinitWorker(workers[i]);
            for (i = _nbWorkers; i < nbWorkers; i++)
                workers[i] = CreateWorker(i);
            //Assigner taches chemin critiques
            InitHeuristic(tasks, &G, &Nplus, &GSize, &NplusSize, 0);			
            TasksCriticalPathWay = (int *)malloc(0);
        }
    }while(Cmax > timeBound && _nbWorkers <= WORKERS_MAX);

    free(tableauRJ);
    free(TasksCriticalPathWayRecop);
    free(TasksCriticalPathWay);
    free(G);
    free(Nplus);
    if (Cmax > timeBound)
    {
        ClearWorkers(&workers);
        return NULL;
    }

    return workers;
}

Worker** HeuristicCalculation(Station** stations, Task** tasks, Task** G, Task** Nplus, Worker** workers, int GSize, int NplusSize, int (*selectionFunction)(Task**, int, Worker**))
{
    int i;
    int compteurTasksDone = 0;
    int SetTask = 0;
    int newNplusSize;
    int chosenTaskIndex;
    int nbWorkersToAssign;
    int NbWorkersAssigned, SetAllTasks, CompteurTasksToDo, beginningTime;
    Task ** TasksDone = (Task **)malloc(0);
    Task ** TasksToDo = (Task **)malloc(0);
    int * tableauWorkers;
    float DateDispo;
    do{
        while (NplusSize > 0)
        {
            SetTask = 0;
            NbWorkersAssigned = 0;
            SetAllTasks = NplusSize;
            CompteurTasksToDo = 0;
            beginningTime = -1;

            while (NplusSize != 0)
            {
                SetTask = 0;
                //les heuristiques influent dans le choix de la prochaine tâche
                chosenTaskIndex = (*selectionFunction)(Nplus, NplusSize, workers); // select the task to process
                nbWorkersToAssign = Nplus[chosenTaskIndex]->nbWorkersToAssign;

                //creation of a table where we put the order of the workers
                tableauWorkers = (int *)malloc(sizeof(int)*nbWorkersToAssign);
                DateDispo = Nplus[chosenTaskIndex]->availabilityTime;

                if (beginningTime == -1)
                    beginningTime = SortAsc(workers, nbWorkersToAssign, DateDispo, tableauWorkers);
                else
                {
                    if (SetParallelTask(workers, nbWorkersToAssign, DateDispo, beginningTime) == 0)
                        beginningTime = SortAsc(workers, nbWorkersToAssign, DateDispo, tableauWorkers);
                    else
                        SetTask = -1;
                }
                if (SetTask == 0){
                    // select workers to assign on a task
                    for (i = 0; i < nbWorkersToAssign; i++)
                        AssignTask(workers[tableauWorkers[i]], Nplus[chosenTaskIndex], beginningTime);

                    compteurTasksDone++;
                    TasksDone = (Task **)realloc(TasksDone, sizeof(Task *)*compteurTasksDone);
                    //On enregistre l'id de la tache placée
                    TasksDone[compteurTasksDone - 1] = Nplus[chosenTaskIndex];
                    ExerciseTask(Nplus[chosenTaskIndex], beginningTime);
                    SetAllTasks--;
                }
                else
                {
                    CompteurTasksToDo++;
                    TasksToDo = (Task **)realloc(TasksToDo, sizeof(Task *)*CompteurTasksToDo);
                    TasksToDo[CompteurTasksToDo - 1] = Nplus[chosenTaskIndex];
                }
                RemoveAt((void**)Nplus, NplusSize--, chosenTaskIndex);

                free(tableauWorkers);
            }
            if (SetAllTasks != 0)
            {
                Nplus = TasksToDo;
                NplusSize = CompteurTasksToDo;
                //On rajoute des taches -> les taches de tasksdone
                newNplusSize = UpdateNplus(NplusSize, &Nplus, &TasksDone, compteurTasksDone);
                NplusSize = newNplusSize;
            }
            else
            {
                NplusSize=UpdateGNplus(&G, &Nplus, &TasksDone, &GSize, compteurTasksDone);
                free(TasksDone);
                TasksDone = (Task **)malloc(0);
                compteurTasksDone = 0;
            }
            TasksToDo = NULL;

        }
    }while (GSize > 0); // while there is still tasks to process

    return workers;
}

int selection_topLong(Task** Nplus, int NplusSize, Worker** workers)
{
    int i = 0;
    int indexMax = 0;
    float maxValue = 0;
    float tmp = 0;
    for (i = 0; i < NplusSize; i++) // find the task with the greatest processing time
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

int selection_topShort(Task** Nplus, int NplusSize, Worker** workers)
{
    int i = 0;
    int indexMin = 0;
    float minValue = 100;
    float tmp = 0;
    for (i = 0; i < NplusSize; i++) // find the task with the greatest processing time
    {
        minValue = minFloat(CalculateCurrentDuration(Nplus[i]), minValue);
        if  (tmp != minValue)
        {
            tmp = minValue;
            indexMin = i;
        }
    }
    return indexMin;
}

int selection_topRandom(Task** Nplus, int NplusSize, Worker** workers)
{
    return rand() % NplusSize; // select at random
}

int selection_test(Task** Nplus, int NplusSize, Worker** workers)
{
    int i;
    for (i = 0; i < NplusSize; i++) // select if minWorkers = _nbWorkers
    {
        if (_nbWorkers == Nplus[i]->workersMin)
        {
            Nplus[i]->nbWorkersToAssign = _nbWorkers;
            return i;
        }
    }

    return selection_topLong(Nplus, NplusSize, workers);
}

int behindDue_Common(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay, float Delta)
{
    //int i;
    int nbWorkers = _nbWorkers;
    /*for (i = 0; i < nbCriticalPathWay; i++)
    {
    tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign = minInt(tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign + 1, minInt(_nbWorkers + 1, tasks[(*TasksCriticalPathWay)[i]]->workersMax));
    }*/
    nbWorkers++;
    return nbWorkers;
}

int behindDue_PWGS(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay, float Delta)
{
    int i;
    int noChange = 0;
    int nbWorkers = _nbWorkers;
    for (i = 0; i < nbCriticalPathWay; i++)
    {
        if (tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign < tasks[(*TasksCriticalPathWay)[i]]->workersMax && tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign < _nbWorkers)
        {
            tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign = minInt(tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign + 1, minInt(_nbWorkers + 1, tasks[(*TasksCriticalPathWay)[i]]->workersMax));
            noChange = 1;
        }
    }
    //pas de solutions malgré les changements, on rend l'ordo initial, on ajoute un worker aux taches du chemin critique et on rajoute un worker
    if (noChange == 0)
    {
        nbWorkers++;
        for (i = 0; i < _nbTasks; i++)
            tasks[i]->nbWorkersToAssign = tasks[i]->workersMin;
    }
    return nbWorkers;
}
/**
 * Delta: Cmax - D
 */
int behindDue_PWGSv2(Task** tasks, int ** TasksCriticalPathWay, int nbCriticalPathWay, float Delta)
{
    int i = 0;
    float delta = 0.0;
    int noChange = 0;
    int nbWorkers = _nbWorkers;
    int nbWoekersAssignedTask = 0;
    int nbWorkersChange = 0;
    while(delta < Delta && i < nbCriticalPathWay)
    {
        nbWoekersAssignedTask = tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign;

        // delta < Delta et Pour toutes les opérations dans le chemin critique
        if ( nbWoekersAssignedTask < tasks[(*TasksCriticalPathWay)[i]]->workersMax && nbWoekersAssignedTask < _nbWorkers)
        {
            // on assigne tous les workers libres
            nbWorkersChange = minInt(tasks[(*TasksCriticalPathWay)[i]]->workersMax, _nbWorkers);
            // mise à jour delta
            delta += CalculateDuration(tasks[(*TasksCriticalPathWay)[i]], nbWoekersAssignedTask) - CalculateDuration(tasks[(*TasksCriticalPathWay)[i]], nbWorkersChange);
            tasks[(*TasksCriticalPathWay)[i]]->nbWorkersToAssign = nbWorkersChange;
            noChange = 1;
        }
        i++;
    }
    //pas de solutions malgré les changements, on rend l'ordo initial, on ajoute un worker aux taches du chemin critique et on rajoute un worker
    if (noChange == 0)
    {
        nbWorkers++;
        for (i = 0; i < _nbTasks; i++)
            tasks[i]->nbWorkersToAssign = tasks[i]->workersMin;
    }
    return nbWorkers;
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

        if (tasks[i]->nbPredecessors == 0) // if a task has no predecessor, it goes in Nplus
        {
            *Nplus = (Task**)realloc(*Nplus, ++(*NplusSize) * sizeof(Task*));
            (*Nplus)[*NplusSize - 1] = tasks[i];
            tasks[i]->availabilityTime = 0;
        }
        else // else it goes in G
        {
            *G = (Task**)realloc(*G, ++(*GSize) * sizeof(Task*));
            (*G)[*GSize - 1] = tasks[i];
        }

        if (first == 1) // if initial initialisation, set initial parameters for workers
        {
            tasks[i]->nbWorkersToAssign = tasks[i]->workersMin;
            minWorkers = maxInt(minWorkers, tasks[i]->nbWorkersToAssign);
        }
    }

    return minWorkers;
}

int InitGNplus(Task*** G, Task*** Nplus, int* GSize)
{
    int i = 0;
    int NplusSize = 0;
    for (i = 0; i < *GSize; i++)
    {
        if ((*G)[i]->nbPredecessors == (*G)[i]->nbPredecessorsDone) // pass from G to Nplus tasks which have no more predecessor to process
        {
            *Nplus = (Task**)realloc(*Nplus, ++NplusSize * sizeof(Task*));
            (*Nplus)[NplusSize - 1] = (*G)[i];
            RemoveAt((void**)*G, (*GSize)--, i--);
        }
    }

    return NplusSize;
}

void ViderG(Task*** G, int* GSize)
{
    int i = 0;
    int NplusSize = 0;
    for (i = 0; i < *GSize; i++)
    {
        RemoveAt((void**)*G, (*GSize)--, i--);
    }

}

int UpdateNplus(int NplusSize, Task*** Nplus, Task *** TasksDone, int nbTasksDone)
{
    int oldNplusSize = NplusSize;
    int i,j,k,trouve;
    for ( i = 0; i<nbTasksDone; i++)
    {
        int NombreSuccesseurs = (*TasksDone)[i]->nbSuccessors;
        Task** Successeurs;
        Successeurs = (*TasksDone)[i]->successors;

        for ( j = 0; j < NombreSuccesseurs; j++)
        {
            if (Successeurs[j]->nbPredecessors == Successeurs[j]->nbPredecessorsDone)
            {
                k = 0;
                trouve = 0;
                while (k<nbTasksDone && trouve == 0)
                {
                    if ((*TasksDone)[k]->id == Successeurs[j]->id)
                        trouve = 1;
                    k++;
                }
                k = 0;
                while (k<oldNplusSize && trouve == 0)
                {
                    if ((*Nplus)[k]->id == Successeurs[j]->id)
                        trouve = 1;
                    k++;
                }
                if (trouve == 0)
                {
                    *Nplus = (Task**)realloc(*Nplus, ++oldNplusSize * sizeof(Task*));
                    (*Nplus)[oldNplusSize - 1] = Successeurs[j];
                }
            }
        }

    }
    return oldNplusSize;
}

int UpdateGNplus(Task*** G, Task*** Nplus, Task *** SetTasks, int* GSize, int nbSetTasks)
{

    int i = 0;
    int NplusSize = 0;
    for (i = 0; i < *GSize; i++)
    {
        if ((*G)[i]->nbPredecessors == (*G)[i]->nbPredecessorsDone) 
            // pass from G to Nplus tasks which have no more predecessor to process
        {
            int j = 0;
            int trouve = 0;
            while (j<nbSetTasks && trouve == 0)
            {
                if ((*SetTasks)[j]->id == (*G)[i]->id)
                    trouve = 1;
                j++;
            }
            if (trouve == 0)
            {
                *Nplus = (Task**)realloc(*Nplus, ++NplusSize * sizeof(Task*));
                (*Nplus)[NplusSize - 1] = (*G)[i];
            }
            RemoveAt((void**)*G, (*GSize)--, i--);
        }
    }

    return NplusSize;
}

void WriteSolution(Worker** workers, char *selection)
{
    FILE * fichier = NULL;
    char *chemin = "../";
    char *extension = ".csv";
    char nomFichier[250];
    int solution=0;
    strcpy(nomFichier, chemin);
    strcat(nomFichier, selection);
    strcat(nomFichier, extension);
    fichier = fopen(nomFichier, "w");
    if (workers == NULL)
    {
        printf("Infeasible Solution.\n");
        fprintf(fichier,"Infeasible Solution.\n");
    }
    else // write for each workers what they do and when
    {
        int i;
        int j;
        for (i = 0; i < _nbWorkers; i++)
        {
            printf("Worker %d: \n", workers[i]->id); // Worker X:
            fprintf(fichier, "Worker %d\n", workers[i]->id);
            for (j = 0; j < workers[i]->nbTasks; j++)
            {
                printf("\t %d: %f -> %f", workers[i]->doneTasks[j]->id, workers[i]->doneTasks[j]->beginningTime, workers[i]->doneTasks[j]->endingTime); // which task done: from this time -> to this time
                fprintf(fichier, ";%d;%f;%f", workers[i]->doneTasks[j]->id, workers[i]->doneTasks[j]->beginningTime, workers[i]->doneTasks[j]->endingTime); // which task done: from this time -> to this time
                if (j + 1 < workers[i]->nbTasks)
                {
                    float spread = workers[i]->doneTasks[j + 1]->beginningTime - workers[i]->doneTasks[j]->endingTime;
                    if (spread > 0)
                    {
                        printf(" ... %f", spread); // ... time waited before next task
                        fprintf(fichier, ";%f", spread); // ... time waited before next task
                    }                    
                }
                printf("\n");
                fprintf(fichier,"\n");
            }
        }
        printf("Found solution: %d workers\n", _nbWorkers); // Found solution: X workers
        fprintf(fichier, "Found solution;%d workers\n", _nbWorkers); // Found solution: X workers
    }
    fclose(fichier);
}

void clean(char *buffer, FILE *fp)
{
    int c;
    char *p = strchr(buffer, '\n');
    if (p != NULL)
        *p = 0;
    else
    {
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}
