#pragma once

#include <stdio.h>
#include <errno.h>

#define BUFF_SIZE 1024
#define CONFIG_PATH "/var/take-break/.config"

static char buffer[BUFF_SIZE];

int read_file(char** out_buffer) {
  FILE* config_file = fopen(CONFIG_PATH, "r+");
  int result;

  if(config_file == NULL) {
    // TODO: Create file if not exist with default options
    perror("Unable to open config file");
    return -1;
  }

  result = fread(buffer, 1, BUFF_SIZE, config_file);
  if(result <= 0) {
    perror("Unable to read config file");
    return -2;
  }

  *out_buffer = buffer;

  return 0;
}
