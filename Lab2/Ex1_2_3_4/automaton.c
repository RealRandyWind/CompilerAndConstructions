#include <stdio.h>
#include <stdlib.h>
#include "intset.h"

/* Do not change EPSILON! There are 128 ASCII characters. */
#define EPSILON 128
/* limit input text size to 2^13 includes '\0' */
#define TEXT_MAXSIZE 8192

typedef unsigned int State;

typedef struct nfa {
  unsigned int nstates;  /* number of states                          */
  State start;           /* number of the start state                 */
  intSet final;          /* set of final (accepting) states           */
  intSet **transition;   /* transition: state x char -> set of states */
} nfa;


typedef struct dfa {
  unsigned int nstates;  /* number of states                          */
  State start;           /* number of the start state                 */
  intSet final;          /* set of final (accepting) states           */
  int **transition;      /* transition: state x char -> state         */
} dfa;

static void *safeMalloc(unsigned int sz) {
  void *ptr = malloc(sz);
  if (ptr == NULL) {
    fprintf(stderr, "Fatal error: malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void *safeRealloc(void *ptr, unsigned int sz) {
  ptr = realloc(ptr, sz);
  if (ptr == NULL) {
    fprintf(stderr, "Fatal error: realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/****** NFA **************************************************/

nfa makeNFA(int nstates) {
  nfa n;
  unsigned int c;
  State s;
  n.nstates = nstates;
  n.start = 0;   /* default start state */
  n.final = makeEmptyIntSet();
  n.transition = safeMalloc(nstates*sizeof(intSet *));
  for (s=0; s < nstates; s++) {
    n.transition[s] = safeMalloc(129*sizeof(intSet));
    for (c=0; c <= EPSILON; c++) {
      n.transition[s][c] = makeEmptyIntSet();
    }
  }
  return n;
}

void freeNFA(nfa n) {
  unsigned int c;
  State s;
  freeIntSet(n.final);
  for (s=0; s < n.nstates; s++) {
    for (c=0; c <= EPSILON; c++) {
      freeIntSet(n.transition[s][c]);
    }
    free(n.transition[s]);
  }
  free(n.transition);
}

nfa readNFA2(FILE *f)
{
  nfa n;
  unsigned int nstates;
  State state;

  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  if (!fscanf(f, "%u\n", &nstates)) {
    fprintf(stderr, "Error: 1st line must be the number of states\n");
    exit(EXIT_FAILURE);
  }
  n = makeNFA(nstates);
  if (!fscanf(f, "%u\n", &n.start)) {
    fprintf(stderr, "Error: 2nd line must be the number of the start state\n");
    exit(EXIT_FAILURE);
  }
  n.final = readIntSetFromFile(f);
  /* read transitions */
  while (fscanf(f, "%u\n", &state) == 1) {
    int c;
    do {
      c = getc(f);
    } while ((c == ' ') || (c == '\t') || (c == '\n'));
    /* c can be a quote ('), 'e' (from eps), or '#' (ascii number) */
    switch (c) {
    case '\'':
      c = fgetc(f);
      fgetc(f);  /* skip closing quote */
      break;
    case 'e': 
      fgetc(f);  /* skip 'p' */
      fgetc(f);  /* skip 's' */
      c = EPSILON;
      break;
    case '#':
      if(!fscanf(f, "%d", &c)) {
        fprintf(stderr, "Error: # must be followed by an unsigned integer\n");
  exit(EXIT_FAILURE);
      }
      break;
    default: 
      fprintf(stderr, "Syntax error in automata file   %c\n", c);
      exit(EXIT_FAILURE);
    }
    n.transition[state][c] = readIntSetFromFile(f);
  }
  fclose(f);
  return n;
}

nfa readNFA(char *filename) {
  return readNFA2(fopen(filename, "r"));
}

void saveNFA(char *filename, nfa n) {
  FILE *f;
  unsigned int c;
  State state;

  f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%d\n%d\n", n.nstates, n.start);
  fprintlnIntSet(f, n.final);
  for (state = 0; state < n.nstates; state++) {
    for (c=0; c<= EPSILON; c++) {
      if (!isEmptyIntSet(n.transition[state][c])) {
        fprintf(f, "%d ", state);
        if (c == EPSILON) {
          fprintf(f, "eps ");
  } else {
          if (c > ' ') {
      fprintf(f, "'%c' ", c); 
    } else {
      fprintf(f, "#%d ", c); 
    }
  }
        fprintlnIntSet(f, n.transition[state][c]);
      }
    }
  }
  fclose(f);
}

/****** DFA **************************************************/

#define NOTRANSITION (-1)

void freeDFA(dfa d) {
  State s;
  freeIntSet(d.final);
  for (s=0; s < d.nstates; s++) {
    free(d.transition[s]);
  }
  free(d.transition);
}

void saveDFA(char *filename, dfa d) {
  FILE *f;
  unsigned int c;
  State state;

  f = fopen(filename, "w");
  if (!f) {
    fprintf(stderr, "Fatal error: failed to open file\n");
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%d\n%d\n", d.nstates, d.start);
  fprintlnIntSet(f, d.final);
  for (state = 0; state < d.nstates; state++) {
    for (c=0; c < 128; c++) {
      if (d.transition[state][c] != NOTRANSITION) {
  if (c > ' ') {
    fprintf(f, "%u '%c' %u\n", state, c, d.transition[state][c]);
  } else {
    fprintf(f, "%u #%d %u\n", state, c, d.transition[state][c]);
  }
      }
    }
  }
  fclose(f);
}

/***********************************************************/
void nfaMoveIntset(intSet* lhs, intSet rhs, unsigned int c, nfa n)
{
  unsigned int x, k, i;
  for (i = 0; i < rhs.size; ++i) {
    x = rhs.bits[i]; k = i*BITS_UINT;
    while(x) {
      if(x%2) { unionIntSet(lhs, n.transition[k][c]); }
      x /= 2; k++;
    }
  }
}

/* returns the EPSILON closure for a given state */
intSet epsilonClosureState(nfa automaton, State s) {
  /* implement the body of this function yourself */
  intSet closure = makeEmptyIntSet();
  intSet complement = makeEmptyIntSet();
  unionIntSet(&closure, automaton.transition[s][EPSILON]);
  while(isEmptyIntSet(complement))
  {
    nfaMoveIntset(&closure, closure, EPSILON, automaton);
  }
  return closure;
}

/* returns the EPSILON closure for a set of states */
intSet epsilonClosureStateSet(nfa automaton, intSet s) {
  /* implement the body of this function yourself */
  intSet closure = makeEmptyIntSet();
  nfaMoveIntset(&closure, s, EPSILON, automaton);
  while(isEmptyIntSet(complement))
  {
    nfaMoveIntset(&closure, closure, EPSILON, automaton);
  }
  return closure;
}

/* DFA construction */
dfa nfa2dfa(nfa n) {
  /* implement the body of this function yourself */
  dfa d;
  d.nstates = 0;
  d.start = 0;
  d.final = makeEmptyIntSet();
  d.transition = NULL;
  return d;
}

/* minimal DFA construction using Brzozowski’s algorithm */
dfa nfa2minimalDFA(nfa n) {
  /* implement the body of this function yourself */
  dfa d;
  d.nstates = 0;
  d.start = 0;
  d.final = makeEmptyIntSet();
  d.transition = NULL;
  return d;
}

nfa nfaMerge(nfa *ns, int nnfas)
{
  nfa n, nk;
  unsigned int nstates, c;
  State start, state;
  int k;
  /* check pre conditions */
  if(!ns || nnfas) { exit(EXIT_FAILURE); }
  /* compute number of states of new nfa given all nfa_k */
  nstates = 1;
  for (k = 0; k < nnfas; ++k) { nstates += ns[k].nstates; }
  n = makeNFA(nstates);
  /* merge the states into the new nfa */
  start = 1;
  for (k = 0; k < nnfas; ++k)
  {
    nk = ns[k];
    /* unifiy final states nfa_k into new nfa */
    unionIntSet(&n.final, nk.final);
    /* unifiy all the state transitions nfa_k into transition new nfa */
    for (state = 0; state < nk.nstates; ++state)
    {
      for (c = 0; c < EPSILON; ++c)
      {
        unionIntSet(&n.transition[start + state][c], nk.transition[state][c]);
      }
    }
    /* compute new start index for nfa_k */
    start += nk.nstates;
  }
  return n;
}

/* construct a lexer using optimized state machines */
void dfaSimulation(const char* text, int* pleng, dfa d)
{
  unsigned int c;
  int leng;
  unsigned int state;
  /* check pre conditions */
  if(!text || !pleng) { exit(EXIT_FAILURE); }
  leng = 0; (*pleng) = leng;
  state = d.start;
  /* evalueate the state transitions */
  while(state != NOTRANSITION && (c = text[leng++]) && c < EPSILON)
  {
    state = d.transition[state][c];
    if(isMemberIntSet(state, d.final)) { (*pleng) = leng; }
  }
}

int main (int argc, char **argv) {
  int nnfas, k, leng;
  char text[TEXT_MAXSIZE];
  nfa n, * ns;
  dfa d;
  State s;

#ifndef EX4
  /* code if not ex4 */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <nfa file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  n = readNFA(argv[1]);
  saveNFA("out.nfa", n);
#endif


#ifdef EX1
  /* code for testing epsilonClosureState */
  for (s=0; s < nfa.nstates; s++) {
    intSet epsclosure = epsilonClosureState(n, s);
    printf("epsclosure(%u)=", s);
    printlnIntSet(epsclosure);
    freeIntSet(epsclosure);
  }
#endif
  
  
#ifdef EX2
  /* code for testing nfa2dfa */
  d = nfa2dfa(n);
  saveDFA("out.dfa", d);
  freeDFA(d);
#endif


#ifdef EX3
  /* code for testing nfa2minimalDFA */
  d = nfa2minimalDFA(n);
  saveDFA("minimal.dfa", d);
  freeDFA(d);
#endif


#ifdef EX4
  /* code for testing dfaSimulation */
  if(!scanf("%i", &nnfas)) { exit(EXIT_FAILURE); }
  ns = (nfa*) malloc(sizeof(nfa) * nnfas);
  if(!ns) { exit(EXIT_FAILURE); }
  for (k = 0; k < nnfas; ++k) { ns[k] = readNFA2(stdin); } /* read all nfa*/
  n = nfaMerge(ns, nnfas);
  d = nfa2minimalDFA(n);
  saveDFA(d); /* just store resulted dfa */
  while(scanf("%*s", TEXT_MAXSIZE, text))
  {
    dfaSimulation(text, &leng, d);
    printf("Accepted %.*s\nRemaining %*.s\n", leng, text, leng, text);
  }
  while(nnfas--) { freeNFA(ns[nnfas]); } /* free all nfa */
  freeDFA(d);
  free(ns);
#endif


  freeNFA(n);
  return EXIT_SUCCESS;
}
