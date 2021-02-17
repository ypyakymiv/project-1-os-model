#ifndef SIMULATION_INCLUDED
#define SIMULATION_INCLUDED

enum event_type {
  JOB_ARRIVE,
  JOB_QUEUE,
  JOB_START,
  JOB_END,
  NET_QUEUE,
  NET_START,
  NET_END,
  DISK1_QUEUE,
  DISK1_START,
  DISK1_END,
  DISK2_QUEUE,
  DISK2_START,
  DISK2_END,
  SIMULATION_END
};

typedef enum event_type event_type;

struct sim_state {
  long time;
  int cpu_load;
  int disk1_load;
  int disk2_load;
  int net_load;
};

void sim_state_init(struct sim_state *);

struct event {
  long time;
  event_type et;
};

struct event *alloc_event(event_type, long);
void free_event(void *);

#endif
