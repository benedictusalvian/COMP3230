#ifndef UTILS_H_LGRWE5AC
#define UTILS_H_LGRWE5AC

/* ========= Don't modify this file =========== */

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdarg.h>
#include "omp.h"

#define PRINT_RED       fprintf(stderr,"\033[0;31m");printf("\033[0;31m");
#define PRINT_RED_B     fprintf(stderr,"\033[1;31m");printf("\033[1;31m");
#define PRINT_GREEN     fprintf(stderr,"\033[0;32m");printf("\033[0;32m");
#define PRINT_GREEN_B   fprintf(stderr,"\033[1;32m");printf("\033[1;32m");
#define PRINT_YELLOW    fprintf(stderr,"\033[0;33m");printf("\033[0;33m");
#define PRINT_YELLOW_B  fprintf(stderr,"\033[1;33m");printf("\033[1;33m");
#define PRINT_BLUE      fprintf(stderr,"\033[0;34m");printf("\033[0;34m");
#define PRINT_BLUE_B    fprintf(stderr,"\033[1;34m");printf("\033[1;34m");
#define PRINT_MAGENTA   fprintf(stderr,"\033[0;35m");printf("\033[0;35m");
#define PRINT_MAGENTA_B fprintf(stderr,"\033[1;35m");printf("\033[1;35m");
#define PRINT_CYAN      fprintf(stderr,"\033[0;36m");printf("\033[0;36m");
#define PRINT_CYAN_B    fprintf(stderr,"\033[1;36m");printf("\033[1;36m");
#define PRINT_DEFAULT   fprintf(stderr,"\033[0m");   printf("\033[0m");

// Thread safe no caching printing functions
void flushed_printf(const char *msg, ...); // use like printf 
void debug_printf(const char *msgHead, const char *msg, ...); 
void err_printf(const char *function, int line, const char *what, ...);

double getCurrentTime();

#endif /* end of include guard: UTILS_H_LGRWE5AC */
