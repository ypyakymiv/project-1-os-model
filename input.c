#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"
#include "input.h"

#define FOREACH_PARAM(PARAM) \
        PARAM(SEED)          \
        PARAM(INIT_TIME)     \
        PARAM(FIN_TIME)      \
        PARAM(ARRIVE_MIN)    \
        PARAM(ARRIVE_MAX)    \
        PARAM(QUIT_PROB)     \
        PARAM(NETWORK_PROB)  \
        PARAM(CPU_MIN)       \
        PARAM(CPU_MAX)       \
        PARAM(DISK1_MIN)     \
        PARAM(DISK1_MAX)     \
        PARAM(DISK2_MIN)     \
        PARAM(DISK2_MAX)     \
        PARAM(NETWORK_MIN)   \
        PARAM(NETWORK_MAX)   \
        PARAM(NUM_PARAMS)    \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define DEFAULT_SEED time(NULL)
#define DEFAULT_INIT_TIME 0
#define DEFAULT_FIN_TIME 100000
#define DEFAULT_ARRIVE_MIN 30
#define DEFAULT_ARRIVE_MAX 100
#define DEFAULT_QUIT_PROB 0.5
#define DEFAULT_NETWORK_PROB 0.5
#define DEFAULT_CPU_MIN 20
#define DEFAULT_CPU_MAX 120
#define DEFAULT_DISK1_MIN 100
#define DEFAULT_DISK1_MAX 300
#define DEFAULT_DISK2_MIN 50
#define DEFAULT_DISK2_MAX 100
#define DEFAULT_NETWORK_MIN 300
#define DEFAULT_NETWORK_MAX 600

enum PARAM {
    FOREACH_PARAM(GENERATE_ENUM)
};

static const char *PARAM_NAME[] = {
    FOREACH_PARAM(GENERATE_STRING)
};

struct sim_params settings = {
    (long) NULL,
    DEFAULT_INIT_TIME,
    DEFAULT_FIN_TIME,
    DEFAULT_ARRIVE_MIN, 
    DEFAULT_ARRIVE_MAX, 
    DEFAULT_QUIT_PROB, 
    DEFAULT_NETWORK_PROB, 
    DEFAULT_CPU_MIN, 
    DEFAULT_CPU_MAX, 
    DEFAULT_DISK1_MIN, 
    DEFAULT_DISK1_MAX, 
    DEFAULT_DISK2_MIN, 
    DEFAULT_DISK2_MAX, 
    DEFAULT_NETWORK_MIN, 
    DEFAULT_NETWORK_MAX
  };

void set_token(char *param_name, char *param_value) {
  int index;
  for(index = 0; index < NUM_PARAMS; index++) {
    if(strcmp(PARAM_NAME[index], param_name) == 0)
      break;
  }

  // error check string to value

  switch(index) {
    case SEED: 
      settings.seed = atoi(param_value);
      break;
    case INIT_TIME: 
      settings.init_time = atoi(param_value);
      break;
    case FIN_TIME: 
      settings.fin_time = atoi(param_value);
      break;
    case ARRIVE_MIN: 
      settings.arrive_min = atoi(param_value);
      break;
    case ARRIVE_MAX: 
      settings.arrive_max = atoi(param_value);
      break;
    case QUIT_PROB: 
      settings.quit_prob = atof(param_value);
      break;
    case NETWORK_PROB: 
      settings.net_prob = atof(param_value);
      break;
    case CPU_MIN: 
      settings.cpu_min = atoi(param_value);
      break;
    case CPU_MAX: 
      settings.cpu_max = atoi(param_value);
      break;
    case DISK1_MIN: 
      settings.disk1_min = atoi(param_value);
      break;
    case DISK1_MAX: 
      settings.disk1_max = atoi(param_value);
      break;
    case DISK2_MIN: 
      settings.disk2_min = atoi(param_value);
      break;
    case DISK2_MAX: 
      settings.disk2_max = atoi(param_value);
      break;
    case NETWORK_MIN: 
      settings.net_min = atoi(param_value);
      break;
    case NETWORK_MAX: 
      settings.net_max = atoi(param_value);
      break;
    case NUM_PARAMS: 
      break;
  }  
}


void read_params(char *filename) {
  settings.seed = DEFAULT_SEED; // initialize params to default seed
  FILE *input_file = ec_fopen(filename, "r");
  char *line = ec_malloc(sizeof(char) * READ_BUFFER_SZ);
  while(fgets(line, READ_BUFFER_SZ, input_file)) {
    char *param_name = strtok(line, " ");
    char *param_value = strtok(NULL, " ");
    set_token(param_name, param_value);
  }
  fclose(input_file);
  free(line);
}
