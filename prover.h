/*
	prover.h: Defines the functions for prover.c
*/

#ifndef __PROVER_H
#define __PROVER_H

/*CONSTANTS*/

#define MAXPRED 50
#define MAXPARAM 10
#define MAXSENT 100000
#define MAXSUB 1000
#define MAXSTRLEN 200


/*STRUCT DEFINITIONS*/

typedef struct {
    /* Predicate name */
    char name[32];
    /* Number of parameters the predicate requires */
    int numparam;
} Predicate;

typedef struct {
    /* Storage for when the parameter is a constant */
    char con[16];
    /* Storage for when the parameter is a variable */
    int var;
} Parameter;

typedef struct {
    /* comment from input file */
    char comment[MAXSTRLEN];
    /* set to true if sentence came from negated part of knowledge base */
    int refutePart;
    /* List of predicates in sentence (indexes into Predicate array) */
    int pred[MAXPRED];
    /* neg[i] set to 1 if predicate indexed by pred[i] is negated */
    int neg[MAXPRED];
    /* Stores the number of predicates for this sentence */
    int num_pred;
    /* List of parameters for each predicate */
    Parameter param[MAXPRED][MAXPARAM];
} Sentence;

typedef struct {
    Parameter *var;
    Parameter *val;
} Assignment;

typedef struct {
	int sent1;
	int sent2;
	unsigned int priority;
} QueueObject;

/*FUNCTION HEADERS*/

/* Allow user to enter a sentence to be added to KB */
void AddKBSentence(void);

/* Add a predicate to the predicate list */
int AddPredicate(char *name, int numparam);

/* TODO */
void AddPredicates(int destSent, int srcSent, int from, int to);

/* TODO Explanation Comment */
void addPredicateWithSkip(int destSent, int srcSent, int skipPred);

/* Add a sentence to the KB */
void AddSentence(int neg[MAXPRED], int pred[MAXPRED],
                 char param[MAXPRED][MAXPARAM][16],  int snum, char *leftover);

/* TODO Explanation Comment */
void AddSentenceFromResolution(int s1, int s2, int p1, int p2, Assignment *theta, int numAssign);

/* Returns true if the parameter is a constant */
int constant(Parameter param);

/* Returns true if the parameter is empty */
int empty(Parameter *param);

/* Set the KB to empty */
void InitializeKB(void);

/* Load a KB from a text file */
void LoadKB(void);

/* Order sentences by number of preds in common that are also negated.*/
int *OrderByPreds(int rSent, int rPreds[]);

/* TODO Explanation Comment */
void performSubstitutions(int s, Assignment *theta ,int numAssign);

/* TODO Explanation Comment */
void printAssignments(Assignment *theta, int numAssign);

/* TODO Explanation Comment */
void printParam(Parameter p);

/* User enters a the negation of their query.  This is added to KB, and then KB is resolved to find solution */
void ProveQuery(void);

/* Read in a KB from a text file */
int ReadKB(char *filename);

/* TODO Explanation Comment */
void replaceVar(Parameter *start, Parameter *end, int var, int val);

/* You must write this function */
void Resolve(void);

/* You must write this function */
void ResolveHeuristic();

/* You must write this function */
void ResolveRandom();

/* Print the current KB to the screen */
void ShowKB(void);

/* Standardize apart (Makes sure that each sentence has unique variables) */
void Standardize(char param[MAXPRED][MAXPARAM][16],
                 Parameter sparam[MAXPRED][MAXPARAM],
                 int pred[MAXPRED], int snum);

/* TODO */
void StandardizeApartVariables(int s);

/* Convert text version of a sentence into internal representation */
int StringToSentence(char *line);

/* TODO */
void tryResolution(int sent1, int sent2);

/* TODO Explanation Comment */
int UnifyPred(int sent1, int p1, int sent2, int p2, Assignment *Theta);

/* Returns true if the parameter is a variable */
int variable(Parameter param);

#endif
