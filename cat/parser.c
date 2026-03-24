#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "cat_proc.h"
#include "read.h"

int sravni_str(char *str1, char *str2) {
  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i]) i++;
  return str1[i] == '\0' && str2[i] == '\0';
}
int is_option(char *arg) {
  int is_option = 0;
  if (arg[0] == '-') {
    if (arg[1] != '\0') is_option = 1;
  }
  return is_option;
}

int set_option_flag(Option *options, int options_count, char *arg) {
  int i = 0;
  int found = 0;
  while (i < options_count) {
    if (sravni_str(arg, options[i].name) ||
        (options[i].long_name != NULL &&
         sravni_str(arg, options[i].long_name))) {
      *options[i].bool_state_pointer = 1;
      found = 1;
    }
    i++;
  }
  return found;
}

int find_file_start(int argc, char **argv) {
  int i = 1;
  while (i < argc && is_option(argv[i])) {
    i++;
  }
  return i;
}

void parse_args(int argc, char **argv, Option *options, int options_count,
                char ***input_files, int *num_files) {
  *num_files = 0;
  int i = 1;
  *input_files = NULL;
  int file_start = find_file_start(argc, argv);

  while (i < file_start) {
    char *arg = argv[i];
    int found = 0;
    if (arg[0] == '-') {
      if (arg[1] != '-') {
        int j = 1;
        while (arg[j] != '\0') {
          char flag = arg[j];
          char flag_str[3] = "-";  //"-b\0"
          flag_str[1] = flag;
          flag_str[2] = '\0';

          int one_found = 0;
          if ((one_found = set_option_flag(options, options_count, flag_str)) ==
              1)
            found = 1;
          j++;
        }
      } else {
        found = set_option_flag(options, options_count, arg);
      }
      if (!found) printf("wtf");
    }
    i++;
  }

  if (file_start < argc) {
    *num_files = argc - file_start;
    *input_files = malloc((*num_files + 1) * sizeof(char *));
    int j = 0;
    while (j < *num_files) {
      (*input_files)[j] = argv[file_start + j];
      j++;
    }
    (*input_files)[*num_files] = NULL;
  }
}

char *primenit_func(char *text, Config_cat *processors, int num) {
  char *current_buf = text;
  for (int i = 0; i < num; i++) {
    if (*(processors[i].bool_state_pointer)) {
      char *out_text = processors[i].func(current_buf);
      if (current_buf != text) {
        free(current_buf);
      }
      current_buf = out_text;
    }
  }
  return current_buf;
}

int main(int argc, char **argv) {
  State state = {0};
  Option cat_options[] = {{"-b", "--number-nonblank", &state.is_b, no_args},
                          {"-e", NULL, &state.is_e, no_args},
                          {"-E", NULL, &state.is_E, no_args},
                          {"-n", "--number", &state.is_n, no_args},
                          {"-s", "--squeeze-blank", &state.is_s, no_args},
                          {"-t", NULL, &state.is_t, no_args},
                          {"-T", NULL, &state.is_T, no_args},
                          {"-v", NULL, &state.is_v, no_args}};
  Config_cat cat_processors[] = {
      {squeeze, &state.is_s},        {change_tab, &state.is_t},
      {change_tab, &state.is_T},     {num_non_empty, &state.is_b},
      {num_all, &state.is_n},        {show_symbols, &state.is_v},
      {change_end_str, &state.is_e}, {change_end_str, &state.is_E}};
  int options_count = (int)(sizeof(cat_options) / sizeof(Option));
  int proccesors_num = (int)(sizeof(cat_processors) / sizeof(Config_cat));

  char **input_files = NULL;
  int num_files;
  parse_args(argc, argv, cat_options, options_count, &input_files, &num_files);
  if (state.is_b && state.is_n) state.is_b = 0;

  char *text = NULL;
  int show = 0;
  if (state.is_v || state.is_e || state.is_t) show = 1;
  if (num_files > 1)
    text = read_files_bin(input_files, num_files, show);
  else if (num_files == 1)
    text = read_file_bin(input_files[0], show);

  if (input_files) free(input_files);

  if (text != NULL) {
    char *final_text = primenit_func(text, cat_processors, proccesors_num);
    printf("%s", final_text);
    if (final_text != text) {
      free(text);
    }
    free(final_text);
  }
  return 0;
}
