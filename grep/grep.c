#include "grep.h"

int main(int argc, char** argv) {
  Flags flag = {0};
  parser(argc, argv, &flag);

  while (optind < argc) {
    out(argv, &flag);
    optind++;
  }
  return 0;
}
void parser(int argc, char** argv, Flags* flag) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        flag->e = 1;
        strcat(flag->str_regex, optarg);
        strcat(flag->str_regex, "|");
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      case 'f':
        flag->f = 1;
        strcpy(flag->file_name, optarg);
        f_flag(flag);
        break;
      default:
        fprintf(stderr, "what is %c\n", optopt);
        break;
    }
    if (flag->v && flag->o) flag->o = 0;
  }
  if (!flag->e && !flag->f) {
    if (argc > optind) strcat(flag->str_regex, argv[optind]);
    optind++;
  }
  if (flag->e || flag->f) {
    flag->str_regex[strlen(flag->str_regex) - 1] = '\0';
  }
  if (argc - optind > 1) flag->c_flag = 1;
}

void process_string(Flags* flag, int* str_number, regex_t* regex,
                    regmatch_t* start, char** argv, int* counter) {
  int ne_sovpalo = regexec(regex, flag->str, 1, start, 0);
  *str_number = *str_number + 1;
  if (flag->o) strcpy(flag->o_str, flag->str);
  // l out
  if ((!ne_sovpalo || flag->v) && flag->c_flag && !flag->l && !flag->h &&
      !flag->c && !flag->f)
    printf("%s:", argv[optind]);
  if (!ne_sovpalo) counter++;
  if (flag->v) ne_sovpalo = !ne_sovpalo;
  if (!ne_sovpalo && !flag->l && !flag->c && !flag->n && !flag->o) {
    printf("%s", flag->str);
    if (flag->str[strlen(flag->str) - 1] != '\n') printf("\n");
  }
  if (!ne_sovpalo && flag->n && !flag->c && !flag->l) {
    if (flag->o)
      printf("%d:", *str_number);
    else
      printf("%d:%s", *str_number, flag->str);
    if (flag->str[strlen(flag->str) - 1] != '\n') printf("\n");
  }
  // o out
  if (!ne_sovpalo && flag->o && !flag->l && !flag->c) {
    char* p = flag->o_str;
    while ((regexec(regex, p, 1, start, 0) == 0)) {
      printf("%.*s\n", (int)(start->rm_eo - start->rm_so), p + start->rm_so);

      p += start->rm_eo;
    }
  }
}
void print(Flags* flag, int counter, char** argv, int str_number) {
  if (flag->l && counter <= 1 && flag->v) printf("%s\n", argv[optind]);
  if (flag->l && counter >= 0 && !flag->c) printf("%s\n", argv[optind]);
  if (flag->c && flag->c_flag && !flag->h) printf("%s:", argv[optind]);
  if (flag->c && !flag->l && !flag->v) printf("%d\n", counter);
  if (flag->c && !flag->l && flag->v) printf("%d\n", str_number - counter);
  if (flag->c && flag->l) {
    if (counter > 0) {
      counter = 1;
      printf("%d\n%s\n", counter, argv[optind]);
    } else {
      printf("%d\n", counter);
    }
  }
}
void out(char** argv, Flags* flag) {
  regex_t regex;
  regmatch_t start;
  int counter = 0;
  int str_number = 0;
  int flag_i = REG_EXTENDED;
  if (flag->i) flag_i = REG_EXTENDED | REG_ICASE;
  regcomp(&regex, flag->str_regex, flag_i);

  FILE* file = fopen(argv[optind], "rb");
  if (file) {
    while (fgets(flag->str, 1000, file) != NULL) {
      process_string(flag, &str_number, &regex, &start, argv, &counter);
    }
    regfree(&regex);

    print(flag, counter, argv, str_number);

    fclose(file);
  } else {
    regfree(&regex);
    if (!flag->s) fprintf(stderr, "file %s is not exist\n", argv[optind]);
  }
}
void f_flag(Flags* flag) {
  FILE* file;
  file = fopen(flag->file_name, "rb");
  if (file != NULL) {
    while (!feof(file)) {
      if (fgets(flag->str, 1000, file) != NULL) {
        if (flag->str[strlen(flag->str) - 1] == '\n' &&
            strlen(flag->str) - 1 != 0)
          flag->str[strlen(flag->str) - 1] = '\0';
        strcat(flag->str_regex, flag->str);
        strcat(flag->str_regex, "|");
      }
    }
    fclose(file);
  }
}