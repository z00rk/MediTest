

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "octopus.h"
#include "websocket.h"
#include <pthread.h>

struct s_controller_context {
  pthread_t job;
  struct s_ws_server *server;
  struct s_octopus_context *octopus;
};

struct s_controller_context *controller_init (void);
#endif /** _CONTROLLER_H_ **/
