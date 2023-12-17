#pragma once
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000

static char line[MAX_LINE];

static int get_line(char* input) {
  static int i = 0;
  char* ptr = line;
  
  while(input[i] != '\0') {
    if(input[i] == '\n') {
      ++i;
      return 0;
    }
    *ptr++ = input[i];
    ++i;
  }

  return EOF;
}

int parse_seconds(char* content, long long* out_seconds) {
  while(get_line(content) != EOF) {
    if(line[0] != '#') {
      *out_seconds = atoll(line);
      return 0;
    }
  }

  return -1;
}
