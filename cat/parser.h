#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>

typedef char *(*processor_cat)(char *);

typedef enum { text_group, no_args, need_args } Option_type;

typedef struct {
  // cat
  int is_b;
  int is_e;
  int is_n;
  int is_s;
  int is_t;
  int is_v;
  int is_E;
  int is_T;

  char **input_files;
  int num_files;
  int line_num;
  int prev_empty;

} State;

typedef struct {
  char *name;
  char *long_name;
  int *bool_state_pointer;
  // int ***strings_state_pointer;
  Option_type type;

} Option;

typedef struct {
  processor_cat func;
  int *bool_state_pointer;
} Config_cat;

int sravni_str(char *str1, char *str2);
void parse_args(int argc, char **argv, Option *options, int options_count,
                char ***input_files, int *num_files);

#endif