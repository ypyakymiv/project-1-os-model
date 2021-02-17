#ifndef INPUT_INCLUDE
#define INPUT_INCLUDE

struct sim_params {
  long seed;
  long init_time;
  long fin_time;
  long arrive_min;
  long arrive_max;
  double quit_prob;
  double net_prob;
  long cpu_min;
  long cpu_max;
  long disk1_min;
  long disk1_max;
  long disk2_min;
  long disk2_max;
  long net_min;
  long net_max;
};  

void read_params(char *filename);

#endif
