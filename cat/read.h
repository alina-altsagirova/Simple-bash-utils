#include <stdio.h>
#ifndef READ_H
#define READ_H

char* read_file_bin(char* file_name, int show);
char* read_files_bin(char** file_names, int files_count, int show);

#endif