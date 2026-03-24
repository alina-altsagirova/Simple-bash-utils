#include "read.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

char* read_string(FILE* stream, int bool_enter) {
  int bool_loop = 1;
  size_t buf_size = 256;
  char* string_buf = calloc(buf_size, sizeof(char));
  char* ext_buf = NULL;
  int sym = 0;
  size_t i = 0;
  while (bool_loop == 1 && (sym = fgetc(stream)) != EOF) {
    if (i > buf_size - 1) {
      buf_size *= 2;
      if (!(ext_buf = realloc(string_buf, (buf_size * sizeof(char)))))
        free(string_buf);
      string_buf = ext_buf;
    }
    string_buf[i] = (char)sym;
    if (bool_enter && (char)sym == '\n') bool_loop = 0;
    i++;
  }
  string_buf[i] = '\0';
  if (i == 0 && string_buf) {
    free(string_buf);
    string_buf = NULL;
  }
  return string_buf;
}

char* str_cat(char* dest, char* source) {
  if (dest == NULL || source == NULL) {
    return NULL;
  }
  char* final_str = NULL;
  int dest_len = 0;
  int source_len = 0;

  while (dest[dest_len] != '\0') dest_len++;
  while (source[source_len] != '\0') source_len++;

  size_t needed_size = (dest_len + source_len + 1);
  final_str = malloc(needed_size);
  int i = 0;
  int k = 0;
  while (i < dest_len) {
    final_str[i] = dest[i];
    i++;
  }
  while (k < source_len) {
    final_str[i] = source[k];
    i++;
    k++;
  }
  final_str[i] = '\0';
  return final_str;
}

char* read_file_bin(char* file_name, int show) {
  FILE* file = fopen(file_name, "rb");
  if (!file) {
    return NULL;
  } else {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    size_t buf_size = 2 * (size_t)size + 1;
    char* buf = malloc(buf_size);

    int c;
    size_t i = 0;
    while ((c = getc(file)) != EOF && i < buf_size - 1) {
      if (c == '\0') {
        if (show) {
          buf[i++] = '^';
          buf[i++] = '@';
          printf("%c", c);
        } else
          buf[i++] = ' ';
      } else {
        buf[i++] = (char)c;
      }
    }
    buf[i] = '\0';

    fclose(file);
    return buf;
  }
}

char* read_files_bin(char** file_names, int files_count, int show) {
  int i = 0;
  char* text_buf = malloc(sizeof(char) * 256);
  text_buf[0] = '\0';

  while (i < files_count) {
    char* file_buf = read_file_bin(file_names[i], show);

    if (!file_buf) {
      free(text_buf);
      return NULL;
    }
    char* ext_buf = str_cat(text_buf, file_buf);
    free(file_buf);
    free(text_buf);
    if (!ext_buf) return 0;
    text_buf = ext_buf;
    i++;
  }
  return text_buf;
}