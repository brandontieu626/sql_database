#ifndef PARSER_HELPERS_H
#define PARSER_HELPERS_H
#include <iostream>
#include <string.h>
using namespace std;

const int ROWS=60;
const int COLUMNS=50;

//Return fail or success of given state
bool fail_or_success(int table[][COLUMNS],int current_state);

//Mark cells from int a to int b at state with next state
void mark_cells(int table[][COLUMNS],int state,int a, int b,int next_state);;

//Mark success at state
void mark_success(int table[][COLUMNS],int state);




#endif // PARSER_HELPERS_H
