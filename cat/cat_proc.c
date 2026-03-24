#include "cat_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *add_to_str(int count_line) {
  char space_symbol = ' ';
  char str_num[7];
  char *final = malloc(8 * sizeof(char));
  int fi = 0;

  int num_len = snprintf(str_num, sizeof(str_num), "%d", count_line);
  int space_num = 6 - num_len;
  for (int i = 0; i < space_num; i++) final[fi++] = space_symbol;
  for (int i = 0; i < num_len; i++) final[fi++] = str_num[i];
  final[fi++] = '\t';
  final[fi++] = '\0';

  return final;
}

char *num_non_empty(char *buffer) {
  int max_num_lines = 1;
  int in_line_count = 0;
  size_t size = strlen(buffer);
  for (unsigned int i = 0; i < size; i++) {
    if (buffer[i] != '\n' && in_line_count == 0) {
      max_num_lines++;
      in_line_count = 1;
    } else if (buffer[i] == '\n') {
      in_line_count = 0;
    }
  }

  size_t final_size = size + (max_num_lines * 8) + 1;
  char *final = malloc(final_size);

  int count_line = 1;
  int bi = 0;
  int fi = 0;
  int in_line = 0;

  while (buffer[bi] != '\0') {
    if (buffer[bi] != '\n' && in_line == 0) {
      char *add = add_to_str(count_line);
      for (unsigned int i = 0; i < strlen(add); i++) final[fi++] = add[i];
      free(add);
      in_line = 1;
      count_line++;
    }

    final[fi] = buffer[bi];
    fi++;
    if (buffer[bi] == '\n') {
      in_line = 0;
    }
    bi++;
  }
  final[fi] = '\0';
  return final;
}

char *change_end_str(char *buffer) {
  int max_num_enter = 1;
  size_t size = strlen(buffer);
  for (unsigned int i = 0; i < size; i++) {
    if (buffer[i] == '\n') max_num_enter++;
  }
  size_t final_size = size + 1 + (max_num_enter * 2);
  char *final = malloc(final_size);

  int bi = 0;
  int fi = 0;
  int n_count = 0;
  while (buffer[bi] != '\0') {
    if (buffer[bi] == '\n') {
      n_count++;
      final[fi++] = '$';
      final[fi++] = buffer[bi];
      bi++;
    } else {
      final[fi++] = buffer[bi++];
    }
  }
  final[fi] = '\0';
  return final;
}

char *num_all(char *buffer) {
  int max_num_lines = 1;
  size_t size = strlen(buffer);
  for (unsigned int i = 0; i < size; i++) {
    if (buffer[i] == '\n') max_num_lines++;
  }

  size_t final_size = size + (max_num_lines * 8) + 1;
  char *final = malloc(final_size);

  int count_line = 1;
  int bi = 0;
  int fi = 0;
  int line_start = 1;

  while (buffer[bi] != '\0') {
    if (line_start) {
      char *add = add_to_str(count_line);
      for (unsigned int i = 0; i < strlen(add); i++) final[fi++] = add[i];
      free(add);
      line_start = 0;
    }
    final[fi] = buffer[bi];
    fi++;
    if (buffer[bi] == '\n') {
      count_line++;
      line_start = 1;
    }
    bi++;
  }
  final[fi] = '\0';
  return final;
}

char *squeeze(char *buffer) {
  int n_count = 0;
  size_t size = strlen(buffer);
  char *final_text = malloc(size + 1);
  final_text[0] = '\0';
  int buf_i = 0;
  int final_i = 0;

  while (buffer[buf_i] != '\0') {
    if (buffer[buf_i] != '\n') {
      final_text[final_i] = buffer[buf_i];
      final_i++;
      buf_i++;
      n_count = 0;
    } else {
      if (n_count < 2) {
        final_text[final_i] = '\n';
        final_i++;
      }
      buf_i++;
      n_count++;
    }
  }
  final_text[final_i] = '\0';
  return final_text;
}

char *change_tab(char *buffer) {
  int t_count = 0;
  size_t size = strlen(buffer);
  for (unsigned int i = 0; i < size; i++) {
    if (buffer[i] == '\t') t_count++;
  }

  size_t final_size = size + 1 + (t_count * 2);
  char *final = malloc(final_size);

  int bi = 0;
  int fi = 0;
  while (buffer[bi] != '\0') {
    if (buffer[bi] == '\t') {
      final[fi] = '^';
      fi++;
      final[fi] = 'I';
      fi++;
      bi++;
    } else {
      final[fi++] = buffer[bi++];
    }
  }
  final[fi] = '\0';
  return final;
}

char *show_symbols(char *buffer) {
  size_t size = strlen(buffer);
  char *final_buffer = malloc(size * 4 + 1);
  size_t pos = 0;
  for (size_t i = 0; i < size; i++) {
    unsigned char c = (unsigned char)buffer[i];

    if (c >= 32 && c <= 126)
      final_buffer[pos++] = c;  // norm ascii
    else if (c <= 31) {         // upravlyayushie
      if (c == 10)
        final_buffer[pos++] = '\n';
      else if (c == 9)
        final_buffer[pos++] = '\t';
      else {
        final_buffer[pos++] = '^';
        final_buffer[pos++] = (char)(c + 64);
      }
    } else if (c == 127) {  // del
      final_buffer[pos++] = '^';
      final_buffer[pos++] = '?';
    } else {
      final_buffer[pos++] = 'M';
      final_buffer[pos++] = '-';
      unsigned char low = c - 128;
      if (low >= 32 && low <= 126)
        final_buffer[pos++] = low;
      else if (low <= 31) {
        final_buffer[pos++] = '^';
        final_buffer[pos++] = (char)(low + 64);
      } else if (low == 127) {
        final_buffer[pos++] = '^';
        final_buffer[pos++] = '?';
      }
    }
  }
  final_buffer[pos] = '\0';
  return final_buffer;
}