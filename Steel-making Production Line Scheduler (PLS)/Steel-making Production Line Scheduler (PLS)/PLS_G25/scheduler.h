#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void fcfs();
void sjf();
void scheduler_main(int *pipefd);
void addPERIOD(char *start_date, char *end_date);
void addORDER(char *order_num, char *due_date, int quantity, char *product_name);
void addBATCH(char *file);
void process(char *line, bool add_batch);
void report(char *algorithm);
void release();