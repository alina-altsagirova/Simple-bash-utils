#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
  char str[10000];
  char str_regex[10000];
  char c_flag;
  char o_str[10000];
  char file_name[10000];
} Flags;

void parser(int argc, char** argv, Flags* flag);
void process_string(Flags* flag, int* str_number, regex_t* regex,
                    regmatch_t* start, char** argv, int* counter);
void print(Flags* flag, int counter, char** argv, int str_number);
void out(char** argv, Flags* flag);
void f_flag(Flags* flag);
#endif