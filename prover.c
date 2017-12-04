#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "prover.h"
#include "PriorityQueue.h"
/* GLOBAL VARIABLES */

double rTime, hTime;
int rSteps, hSteps;
int MaxStartPreds = 0;

int RefuteFlag=0;
int sentptr;
Sentence sentlist[MAXSENT];
Predicate predlist[MAXPRED];
int nextvar;
char *filename;


/* FUNCTIONS */


/* Allow user to enter a sentence to be added to KB */
void AddKBSentence(void)
{
    char sent[255];

    printf("\nEnter sentence: ");
    fgets(sent,255,stdin);
    StringToSentence(sent);
}

/* Add a predicate to the predicate list.
   For parsing.*/
int AddPredicate(char *name, int numparam) {
    int i;

    i = 0;
    /* Check to see if predicate already in list */
    while(predlist[i].name[0] != '\0' && strcmp(name,predlist[i].name)) i++;
    if(predlist[i].name[0] == '\0') {
        /* Not in predicate list, so add */
        strcpy(predlist[i].name,name);
        predlist[i].numparam = numparam;
    }
    return i;
}

/* Combine predicates together. */
void AddPredicates(int destSent, int srcSent, int from, int to)
{
    int i;
    for (i = from; i < to; i++){
        sentlist[destSent].pred[sentlist[destSent].num_pred] = sentlist[srcSent].pred[i];
        sentlist[destSent].neg[sentlist[destSent].num_pred] = sentlist[srcSent].neg[i];
        memcpy(sentlist[destSent].param[sentlist[destSent].num_pred], sentlist[srcSent].param[i], sizeof(Parameter));
        sentlist[destSent].num_pred++;
    }
}

/* Combine predicates into one predicate set after a resoluation occurs.
   Skips the predicate that was supposed to be remove. */
void addPredicateWithSkip(int destSent, int srcSent, int skipPred)
{
    AddPredicates(destSent, srcSent, 0, skipPred);
    AddPredicates(destSent, srcSent, skipPred+1, sentlist[srcSent].num_pred);
}

/* Add a sentence to the KB */
/* Implementation unknown if correct */
void AddSentence(int neg[MAXPRED], int pred[MAXPRED],
                 char param[MAXPRED][MAXPARAM][16], int snum, char *leftover)
{
    int i;

    Standardize(param,sentlist[sentptr].param,pred,snum);
    for(i=0; i<snum; i++) {
        sentlist[sentptr].pred[i] = pred[i];
        sentlist[sentptr].neg[i] = neg[i];
    }
    if(*leftover == '.')
    {
        leftover++;
        leftover[strlen(leftover)-1]=0; /* get rid of new line char */
        strcpy(sentlist[sentptr].comment,leftover);
    }
    sentlist[sentptr].refutePart = RefuteFlag;
    sentlist[sentptr].num_pred = snum;
    sentptr++;
}

/* Combine two sentences into one after to finish a resolution. */
void AddSentenceFromResolution(int s1, int s2, int p1, int p2, Assignment *theta, int numAssign)
{
    memset(&(sentlist[sentptr]), 0, sizeof(Sentence));
    addPredicateWithSkip(sentptr, s1, p1);
    addPredicateWithSkip(sentptr, s2, p2);
//    printf("Seg Fault time?\n");
    performSubstitutions(sentptr, theta, numAssign);
    StandardizeApartVariables(sentptr);
//    printf("Guess not\n");
    sentptr++;
}

/* Returns true if the parameter is a constant */
int constant(Parameter param) {
    if(param.var <= 0 && param.con[0] != '\0') return 1; else return 0;
}

/* Returns true if the parameter is empty */
int empty(Parameter *param) {
    if(param->var <= 0 && param->con[0] == '\0') return 1; else return 0;
}

/* Return a priority for set of sentences.*/
int getPriority(int sentToCompare, int sentToResolve)
{
    int priority = 0;
    int pc; //Current predicate from sentToCompare.
    int pr; //Current predicate from sentToResolve.
    //int *preds = sentlist[s].pred;
    for(pc=0; pc<sentlist[sentToCompare].num_pred; pc++){
        int curPriority = priority;
        for(pr=0; pr<sentlist[sentToResolve].num_pred; pc++){
            if(sentlist[sentToCompare].pred[pc] == sentlist[sentToResolve].pred[pr]){
                if(sentlist[sentToCompare].neg[pc] !=sentlist[sentToResolve].neg[pr]){
                    priority--;
                    break;
                }
            }
        }
        if(curPriority == priority){
            priority++;
        }
    }
    return priority;
}

/* Set the KB to empty */
void InitializeKB(void) {
    sentptr = 0;
    memset(sentlist,0,MAXSENT*sizeof(Sentence));
    memset(predlist,0,MAXPRED*sizeof(Predicate));
    nextvar = 1;
}

/* Load a KB from a text file */
void LoadKB(void)
{
    char filename[255];

    printf("\nEnter filename: ");
    fgets(filename,255,stdin);
    if(!ReadKB(filename))
        InitializeKB();
}

/* Substitute variables based on previous resolution. */
void performSubstitutions(int s, Assignment *theta, int numAssign)
{
    int x,y,z;

    printAssignments(theta, numAssign);
//    printf("Performing substitutions %d\n", numAssign);
    //printf("Enter x for loop with %d preds\n", sentlist[s].num_pred);
    for(x=0;x<sentlist[s].num_pred; x++){
        //printf("predicate %d\n", x);
        //printf("Enter y for loop with %d params\n", predlist[sentlist[s].pred[x]].numparam);
        for(y=0; y<predlist[sentlist[s].pred[x]].numparam; y++){
            //printf("Parameter %d\n", y);
            //printf("Enter z for loop\n");
            for(z=0; z<numAssign; z++){
                //printf("Check if loop\n");
                if(sentlist[s].param[x][y].var == theta[z].var->var){
//                    printf("Making assignment of %s", variable(*(theta[z].val))?"v":theta[z].val->con);
                    sentlist[s].param[x][y] = *(theta[z].val);
                }
            }
        }
    }
}

/* Print assignments */
void printAssignments(Assignment *theta, int numAssign)
{
    int x;
    for(x=0; x<numAssign; x++){
        printParam(*(theta[x].var));
        printf(" = ");
        printParam(*(theta[x].val));
        printf("\n");
    }
}

/* Print parameters. */
void printParam(Parameter p)
{
    if(constant(p)){
        printf("%s", p.con);
    } else {
        printf("%d", 'a' + (unsigned char)((p.var)%26));
    }
}

/* User enters a the negation of their query.  This is added to KB, and then KB is resolved to find solution */
void ProveQuery(void)
{
    char query[255];

    printf("\nEnter negated query: ");
    fgets(query,255,stdin);
    StringToSentence(query);
    Resolve();
}

/* Read in a KB from a text file */
int ReadKB(char *filename)
{
    FILE *kbfile;
    char line[255];

    kbfile = fopen(filename,"rt");
    if(!kbfile)
    {
        fprintf(stderr,"File %s not found.\n", filename);
        return 0;
    }
    while(fgets(line,255,kbfile) != 0) {
        if(line[0]=='\n') RefuteFlag=1;  /* the rest after the first blank line should be the negated conclusion */
        else if(!StringToSentence(line))
        {
            fprintf(stderr,"Unable to parse line %s\n",line);
            return 0;
        }
    }
    fclose(kbfile);
    return 1;
}

/* Replace a var with a val. */
void replaceVar(Parameter *start, Parameter *end, int var, int val)
{
    while(start != end){
        if(start->var ==  var){
            start->var=val;
        }
        start++;
    }
}

/* Resolve the knowledge base using a random and heuristic resolve,
   then compare the times they took. */
void Resolve(void)
{
    //Set MaxStartPreds for knowing when a kb can't be resolved.
    int s;
    for(s=0; s<sentptr; s++){
        if(sentlist[s].num_pred > MaxStartPreds){
            MaxStartPreds = sentlist[s].num_pred;
        }
    }
    ResolveRandom();
    //Reset KB
    InitializeKB();
    ReadKB(filename);
    ResolveHeuristic();
    printf("Heuristic vs Random ratios:  hSteps/rSteps = %lg, hTime/rTime = %lg\n\n",
           (double)hSteps/(double)rSteps,
           hTime/rTime);
}

/* You must write this function */
void ResolveHeuristic()
{
    struct timeval start, end;
    hTime=0.0;
    hSteps=0;
    QueueObject **resolutions = malloc(sizeof(QueueObject *) * 4096);
    int resSize = 0;
    //Time at start of RandomResolve approach.
    gettimeofday(&start, NULL);

    //Get sentence to resolve.
    int sentToResolve = 0;
    while(1){
        if(sentlist[sentToResolve].num_pred == 0){
            sentToResolve--;
            break;
        }
        sentToResolve++;
    }
    //Order by some heuristic.
    PriorityQueue *ordered = createPriorityQueue();
    int pos;
//printf("****ENTER THE FOR LOOP****\n");
    for(pos=0; pos<sentToResolve; pos++){
        int primPriority = getPriority(pos, sentToResolve);
        int secPriority = sentlist[pos].num_pred;
        addToQueue(ordered, pos, sentToResolve, primPriority, secPriority);
    }
//	printf("****EXIT THE FOR LOOP****\n");

    //Loop until done.
    int curptr;
    int done = 0;
    int success = 1;
    while(!done){
        curptr = sentptr;
//        printf("Getting next pair");
        QueueObject *nextPair = pullFromQueue(ordered);
        if (!nextPair)
        {
//            printf("Queue Empty: Can not resolve\n");
            success = 0;
            break;
        }
//        printf("next pair sent 1: %d\nnext pair sent 2: %d\n",
//                         nextPair->sent1, nextPair->sent2);
        tryResolution(nextPair->sent1, nextPair->sent2);
        hSteps++;
        if(sentlist[sentptr-1].num_pred == 0){
//            printf("Success\n");
            nextPair->primaryPriority = -1;
            resolutions[resSize++] = nextPair;
            break;
        }
        int x;
        for(x=curptr; x<sentptr; x++){
            nextPair->primaryPriority = x;
            resolutions[resSize++] = nextPair;
        }
        
        int pos;
//        printf("****ENTER THE FOR LOOP****\n");
        if(curptr != sentptr){
            for(x=curptr; x<sentptr; x++){
                if(sentlist[x].num_pred > 3 * MaxStartPreds){
                    //printf("Infinite resolutions. Can not resolve\n");
                    done = 1;
                    success = 0;
                    break;
                }
                for(pos=0; pos<x; pos++){
                    int primPriority = getPriority(pos, x);
                    int secPriority = sentlist[pos].num_pred;
                    addToQueue(ordered, pos, x, primPriority, secPriority);
                }
            }
            curptr = sentptr;
        }
//	printf("****EXIT THE FOR LOOP****\n");
    }

    //Get end time.
    gettimeofday(&end, NULL); //Time at end of HeuristicResolve approach.
    int seconds = (end.tv_sec - start.tv_sec);
    double useconds = (end.tv_usec - start.tv_usec)/(1000000000.0);
    hTime=seconds + useconds;
    freeQueue(ordered);
    //Print results.
    printf("HeuristicResolve: #steps = %i, time = %lg\n\n",hSteps, hTime);
    
    if(success){
        printf("Here is the reverse proof\n");
        //printf("resSize is %d\n", resSize);
        int done = 0;
        int i;
        QueueObject *last = resolutions[--resSize];
        while(!done){
            for(i=0; i<resSize; i++){
                if(resolutions[i]->primaryPriority == last->sent2){
                    printf("Sentence%d\n", last->primaryPriority);
                    printf("Sentence%d, Sentence%d\n", last->sent2, last->sent1);
                    last = resolutions[i];
                    done = 1;
                    break;
                }
            }
            if(done){
                done = 0;
            } else {
                break;
            }
        }
    } else {
        printf("A proof could not be made\n");
    }
}

/* You must write this function */
void ResolveRandom()
{
    struct timeval start, end;
    rTime=0.0;
    rSteps=0;

    //Time at start of RandomResolve approach.
    gettimeofday(&start, NULL);

    //Get sentence to resolve.
    int sentToResolve = 0;
    while(1){
        if(sentlist[sentToResolve].num_pred == 0){
            sentToResolve--;
            break;
        }
        sentToResolve++;
    }
    //Order by some heuristic.
    PriorityQueue *ordered = createPriorityQueue();
    int pos;
    srand(time(NULL));
//	printf("****ENTER THE FOR LOOP****\n");
    for(pos=0; pos<sentToResolve; pos++){
        int primPriority = rand() % MAXPRED;
        int secPriority = rand() % MAXPRED;
        addToQueue(ordered, pos, sentToResolve, primPriority, secPriority);
    }
//	printf("****EXIT THE FOR LOOP****\n");

    //Loop until done.
    int curptr;
    int done = 0;
    int success = 1;
    while(!done){
        curptr = sentptr;
//        printf("Getting next pair\n");
        QueueObject *nextPair = pullFromQueue(ordered);
        if (!nextPair)
        {
//            printf("Queue empty. Can not resolve\n");
            success = 0;
            break;
        }
//        printf("next pair sent 1: %d\nnext pair sent 2: %d\n",
//                         nextPair->sent1, nextPair->sent2);
        tryResolution(nextPair->sent1, nextPair->sent2);
        free(nextPair);
        rSteps++;
        if(sentlist[sentptr-1].num_pred == 0){
//            printf("Success\n");
            break;
        }
        int pos;
        int x;
//        printf("****ENTER THE BROKEN FOR LOOP****\n");
        if(curptr != sentptr){
            for(x=curptr; x<sentptr; x++){
                if(sentlist[x].num_pred > 3 * MaxStartPreds){
//                    printf("Infinite resolutions. Can not resolve\n");
                    done = 1;
                    success = 0;
                    break;
                }
                for(pos=0; pos<x; pos++){
                    int primPriority = rand() % MAXPRED;
                    int secPriority = rand() % MAXPRED;
                    addToQueue(ordered, pos, x, primPriority, secPriority);
                }
            }
        }
//	printf("****EXIT THE BROKEN FOR LOOP****\n");
    }

    gettimeofday(&end, NULL); //Time at end of RandomResolve approach.
    freeQueue(ordered);
    int seconds = (end.tv_sec - start.tv_sec);
    double useconds = (end.tv_usec - start.tv_usec)/(1000000000.0);
    rTime= seconds + useconds;

    //Print result.
    printf("RandomResolve: #steps = %i, time = %lg\n\n",rSteps, rTime);
    
    if(success){
        //TODO print proof
    } else {
        printf("A proof could not be made\n");
    }
}

/* Print the current KB to the screen */
void ShowKB(void)
{
    int i,j,k,p;

    printf("\nCurrent Knowledge Base\n");
    for(i=0; i<sentptr; i++)
    {
        printf("%d: ",i);
        for(j=0; j<sentlist[i].num_pred; j++){
            if(sentlist[i].neg[j]) printf("!");
            p = sentlist[i].pred[j];
            printf("%s(",predlist[p].name);
            for(k=0; k<predlist[p].numparam; k++){
                if(constant(sentlist[i].param[j][k]))
                    printf("%s",sentlist[i].param[j][k].con);
                else
                    printf("%c",'a'+(unsigned char) sentlist[i].param[j][k].var%26);
                if(k<predlist[p].numparam-1)
                    printf(",");
                else
                    printf(") ");
            }
        }
        if(strlen(sentlist[i].comment)) printf("  //%s",sentlist[i].comment);
        if(sentlist[i].refutePart) printf("  :from refuted part");
        printf("\n");
    }
    printf("\n");
}


/* Standardize apart (Makes sure that each sentence has unique variables) */
/* Implementation unknown if correct */
void Standardize(char param[MAXPRED][MAXPARAM][16],
                 Parameter sparam[MAXPRED][MAXPARAM],
                 int pred[MAXPRED], int snum)
{
    int i,j,k,sub[256];

    for(i=0; i<256; i++) sub[i] = -1;
    for(k=0; k<snum; k++)
        for(j=0; j<MAXPARAM; j++) {
            i = pred[k];
            if(param[k][j][0] == '\0') continue;
            /*else if(isupper(param[k][j][0])) strcpy(sparam[i][j].con,param[k][j]);*/
            else if(isupper(param[k][j][0])) strcpy(sparam[k][j].con,param[k][j]);
            else {
                if(sub[(unsigned char) param[k][j][0]] == -1) {
                    sub[(unsigned char) param[k][j][0]] = nextvar;
                    sparam[k][j].var = nextvar;
                    nextvar++;
                }
                else {
                    sparam[k][j].var = sub[(unsigned char) param[k][j][0]];
                }
            }
        }
}

/* Ensure all variables are different unless they are supposed to be the same.*/
void StandardizeApartVariables(int s)
{
    int oldVars = nextvar;
    int x,y;

    for(x=0; x<sentlist[s].num_pred; x++){
        for(y=0; y<predlist[sentlist[s].pred[x]].numparam;y++){
            if(sentlist[s].param[x][y].var > 0 && sentlist[s].param[x][y].var<oldVars){
                replaceVar(&(sentlist[s].param[x][y]), &(sentlist[s].param[MAXPRED][MAXPARAM]), sentlist[s].param[x][y].var, nextvar);
                nextvar++;
            }
        }
    }
}

/* Convert text version of a sentence into internal representation */
int StringToSentence(char *line)
{
    char pname[32],param[MAXPRED][MAXPARAM][16];
    int i,j,p,done,neg[MAXPRED],pred[MAXPRED],snum;

    memset(param,0,MAXPRED*MAXPARAM*16*sizeof(char));
    i = 0;
    snum = 0;
    while((line[i] != '\0') && (line[i] != '\n') && (line[i] != '.')){
        /* 'neg' will keep track of whether the predicate is negated or not */
        neg[snum]=0;
        while(isspace(line[i]))
            i++;
        if((line[i] == '\0') || (line[i] == '\n') || (line[i] == '.'))
            break;
        if(line[i] == '!') {
            neg[snum] = 1;
            i++;
            while(isspace(line[i]))
                i++; /* Added by Tim Andersen.  just in case someone puts space(s)
                        between the ! and the beginning of the predicate name */
        }
        /* get predicate name */
        j = i;
        /* while(line[j] != '(' && line[j] != '\0') j++; commented out by Tim Andersen */
        /* The following line added by Tim Andersen to insure that a predicate name only includes text characters */
        while(((line[j] >= 'a') && (line[j] <= 'z')) || ((line[j]>='A') && (line[j]<='Z')))
            j++;
        if(line[j] != '(')
            return 0;  /* we better see the parameter list immediately after the predicate name */
        if(j == i) /* added by Tim Andersen - we better have at least one char in name */
        {
            return 0;
        }
        memset(pname,0,32*sizeof(char));
        strncpy(pname,&line[i],j-i);

        /* get the parameters */
        done = 0;
        p = 0;
        while(!done) {
            i = j+1;
            while(isspace(line[i]))
                i++;
            j = i;
            /* while(line[j] != ',' && line[j] != ')' && line[j] != '\0') j++; commented out by Tim Andersen */
            /* The following line added by Tim Andersen to insure
               that a parameter name only includes text characters */
            while(((line[j] >= 'a') && (line[j] <= 'z')) || ((line[j]>='A') && (line[j]<='Z')))
                j++;
            switch(line[j])
            {
            case ' ':       /* added by Tim Andersen to allow spaces here */
            case ')':
            case ',': strncpy(param[snum][p],&line[i],j-i); p++; break;
                      break;
            default:
                      return 0;
            }
            while(isspace(line[j]))
                j++;
            switch(line[j])
            {
            case ')':
                done = 1;
            case ',':
                break;
            default:
                return 0;
            }
        }
        i = j+1;
        pred[snum] = AddPredicate(pname,p);
        snum++;
    }
    AddSentence(neg,pred,param,snum,&line[i]);
    return 1;
}

/* Attempt resolution of the sentences. */
void tryResolution(int sent1, int sent2)
{
//    printf("tryResolution\n");
    int curptr = sentptr;
    Assignment theta[MAXPARAM];
    int p1, p2;
    for(p1=0; p1<sentlist[sent1].num_pred; p1++){
        for(p2=0; p2<sentlist[sent2].num_pred; p2++){
            int numAssign = UnifyPred(sent1, p1, sent2, p2, theta);
//            printf("numAssign = %d\n", numAssign);
            if(numAssign >= 0){
//                printf("Adding sentence\n");
                AddSentenceFromResolution(sent1, sent2, p1, p2, theta, numAssign);
            }
        }
    }
    return;
}

/* Unify two predicates */
int UnifyPred(int sent1, int p1, int sent2, int p2, Assignment *theta)
{
    int param;
    int numAssign = 0;
//    printf("pred1 = %d\npred2 = %d\n", sentlist[sent1].pred[p1], sentlist[sent2].pred[p2]);
    //make sure predicates are not in the same "state" of negation
    if (sentlist[sent1].neg[p1] == sentlist[sent2].neg[p2]){
//	printf("Negation was the same\n");
        return -1;
    }
    //make sure the predicates aren't the same
    if (sentlist[sent1].pred[p1] !=  sentlist[sent2].pred[p2]){
//	printf("Predicates did not match\n");
        return -1;
    }

    //Store the list of parameters for each predicate
    Parameter param1[MAXPARAM];
    Parameter param2[MAXPARAM];

    *(param1) = *(sentlist[sent1].param[p1]);
    *(param2) = *(sentlist[sent2].param[p2]);

    for(param=0; param<predlist[sentlist[sent1].pred[p1]].numparam; param++){
        int i;
        //Need to walk assignment list and make them.
        for(i=0;i<numAssign;i++){
            if(!memcmp(&(param1[param]), theta[i].var, sizeof(Parameter))){
                param1[param] = *(theta[i].val);
            }
            if(!memcmp(&(param2[param]), theta[i].var, sizeof(Parameter))){
                param2[param] = *(theta[i].val);
            }
        }

//        printf("param1 = %d\nparam2 = %d\n", param1[param], param2[param]);
        if(memcmp(&(param1[param]), &(param2[param]), sizeof(Parameter))){
//            printf("param1 variable :%d\nparam2 variable %d\n", variable(param1[param]), variable(param2[param]));
            if(variable(param1[param])){
		if (numAssign == -1)
			numAssign = 0;
                theta[numAssign].var = &(sentlist[sent1].param[p1][param]);
                theta[numAssign].val = &(sentlist[sent2].param[p2][param]);
		numAssign++;
            }
            // else if(variable(sentlist[sent1].param[p1][param])){
            else if(variable(param2[param])) {
		if (numAssign == -1){
			numAssign = 0;
		}
                theta[numAssign].val = &(sentlist[sent1].param[p1][param]);
                theta[numAssign].var = &(sentlist[sent2].param[p2][param]);
		numAssign++;
            }
            else {
//		    printf("No variable\n");
		    continue;
	    }
        }
    }
//    printf("UnifyPred was successful\n");
    return numAssign;
}

/* Returns true if the parameter is a variable */
int variable(Parameter param) {
    if(param.var > 0 && param.con[0] == '\0') return 1; else return 0;
}


/* MAIN */

int main(int argc, char *argv[])
{
    char choice[64];
    int done;

    srand((unsigned int) time(0));
    if(argc > 2) {
        printf("Usage: prover [filename]\n");
        exit(0);
    }
    InitializeKB();
    if(argc == 2) {
        filename = argv[1];
        if(!ReadKB(filename)){
            printf("Syntax error in knowledge base\n");
            exit(0);
        }
        ShowKB();
        Resolve();
        exit(0);
    }
    done = 0;
    while(!done) {
        do {
            system("clear");
            printf("\nMain Menu");
            printf("\n---------");
            printf("\n(A)dd sentence to database");
            printf("\n(C)lear database");
            printf("\n(L)oad database");
            printf("\n(S)how database");
            printf("\n(P)rove query");
            printf("\n(Q)uit program");
            printf("\n\nChoice: ");
            fgets(choice,64,stdin);
        }
        while(choice[0] != 'a' && choice[0] != 'c' && choice[0] != 'l' &&
              choice[0] != 'p' && choice[0] != 's' && choice[0] != 'q');
        switch(choice[0]) {
        case 'a':
            AddKBSentence();
            break;
        case 'c':
            InitializeKB();
            break;
        case 'l':
            InitializeKB();
            LoadKB();
            break;
        case 's':
            {
                char temp[100];
                ShowKB();
                printf("Press enter to continue... ");
                fgets(temp,100,stdin);
                break;
            }
        case 'p':
            ProveQuery();
            break;
        case 'q':
            done = 1;
            break;
        }
    }
    return 0;
}
