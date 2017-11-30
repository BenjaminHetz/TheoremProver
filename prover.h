#IFNDEF __PROVER_H
#DEFINE __PROVER_H

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
    int numAssign;
} Assignment;


/*FUNCTION HEADERS*/

int *OrderByPreds(int rSent, int rPreds[]);
/* Returns true if the parameter is a constant */
int constant(Parameter param);
/* Returns true if the parameter is a variable */
int variable(Parameter param);
/* Returns true if the parameter is empty */
int empty(Parameter *param);
/* Set the KB to empty */
void InitializeKB(void);
/* Add a predicate to the predicate list */
int AddPredicate(char *name, int numparam);
/* Standardize apart (Makes sure that each sentence has unique variables) */
void Standardize(char param[MAXPRED][MAXPARAM][16],
                 Parameter sparam[MAXPRED][MAXPARAM],
                 int pred[MAXPRED], int snum);
/* Add a sentence to the KB */
void AddSentence(int neg[MAXPRED],
                 int pred[MAXPRED],
                 char param[MAXPRED][MAXPARAM][16],
                 int snum, char *leftover);
/* Convert text version of a sentence into internal representation */
int StringToSentence(char *line);
/* Read in a KB from a text file */
int ReadKB(char *filename);
/* Load a KB from a text file */
void LoadKB(void);
/* Print the current KB to the screen */
void ShowKB(void);
/* Allow user to enter a sentence to be added to KB */
void AddKBSentence(void);
/* TODO Explanation Comment */
int Unify(int sent1, int sent2, Assignment *Theta);
/* You must write this function */
void RandomResolve();
/* Order sentences by number of preds in common that are also negated.*/
int *OrderByPreds(int rSent, int rPreds[]);
/* You must write this function */
void HeuristicResolve();
/* You must write this function */
void Resolve(void);
/* User enters a the negation of their query.  This is added to KB, and then KB is resolved to find solution */
void ProveQuery(void);
/* TODO Explanation Comment */
void addPredicateWithSkip(int destSent, int srcSent, int skipPred);
/* TODO Explanation Comment */
void AddSentenceFromResolution(int s1, int s2, int p1, int p2, Assignment *theta);
/* TODO Explanation Comment */
void performSubstitutions(int s, Assignment *theta);
/* TODO Explanation Comment */
void printAssignments(Assignment *theta);
/* TODO Explanation Comment */
void printParam(Parameter p);
/* TODO Explanation Comment */
void replaceVar(Parameter *start, Parameter *end, int var, int val);
/* TODO */
void StandardizeApartVariables(int s);
/* TODO */
int tryResolution(int sent1, int sent2);
