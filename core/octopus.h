

#ifndef _OCTOPUS_H_
#define _OCTOPUS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "messenger.h"
#include "commander.h"

enum e_octopus_state {
  OCTOPUS_STATE_STOPPED,
  OCTOPUS_STATE_INIT,
  OCTOPUS_STATE_RUNNING,
  OCTOPUS_STATE_LAST
};

struct s_octopus_context {
  pthread_t job;
  enum e_octopus_state state;
  struct s_commander_context *command_ctx;
  struct s_message_context *message_ctx;
  void *user_data;
};

void *octopus_job (void*);
struct s_octopus_context *octopus_init (void *);
int32_t octopus_start (struct s_octopus_context *);
#endif /** _OCTOPUS_H_ **/
