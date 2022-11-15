#ifndef TABLE_FUNCTIONS_H
#define TABLE_FUNCTIONS_H
#include <iostream>
#include <string.h>
using namespace std;

const int MAX_COLUMNS=128;
const int MAX_ROWS=60;

void init_table(int table[][MAX_COLUMNS]);

bool fail_or_success(int table[][MAX_COLUMNS],int current_state);

void mark_cells(int table[][MAX_COLUMNS],int state,char a, char b,int next_state);

void mark_cells(int table[][MAX_COLUMNS],int state,string characters,int next_state);

void mark_success(int table[][MAX_COLUMNS],int state);

void mark_failure(int table[][MAX_COLUMNS],int state);

#endif // TABLE_FUNCTIONS_H
