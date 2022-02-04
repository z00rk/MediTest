

#include <pthread.h>


#include "controller.h"
#include "octopus.h"

int32_t controller_msg_cbk (struct s_message_event *p_msg, void *data)
{
  printf ("received message event \n");

  /** @TODO **/

  return 0;
}

int32_t controller_receive (void *server, int len, char *message, void *data)
{
  struct s_controller_context *context = (struct s_controller_context *) data;
  struct s_octopus_context *octopus;
  struct s_message_context *msg;
  struct s_commander_context *cmd; 
  /**
   * This routine will be responsible of receiving data,
   * parsing the JSON-RPC request, and converting it into
   * a command to the octopus FSM
   **/

  octopus = context->octopus;
  cmd = octopus->command_ctx;
  msg = octopus->message_ctx;

  printf ("received message %s\n", message);
  command_send (cmd, COMMAND_LAST);

  return 0;
}

int32_t controller_close (void *server, void *client, void *data)
{
  struct s_controller_context *context = (struct s_controller_context *) data;

  printf ("connection is closed\n");

  return 0;
}

int32_t controller_connected (void *server, void *client, void *data)
{
  struct  s_controller_context *context = (struct s_controller_context *) data;

  printf ("a client connected to the socket\n");

  return 0;
}

void *controller_job (void *arg)
{
  struct s_controller_context *context = (struct s_controller_context *)arg;

  octopus_start (context->octopus);

  while (1) {


    ws_server_run (context->server);
    usleep (100000);
  }

  return;
}

struct s_controller_context *controller_init (void)
{
  int32_t ret = -1;
  struct s_controller_context *ctrl;

  printf ("[INIT] starting controller \n");

  ctrl = (struct s_controller_context *) calloc (1, sizeof (struct s_controller_context));

  ctrl->octopus = octopus_init (ctrl);

  if (!ctrl->octopus) {
    printf ("error starting the machine \n");
    free (ctrl);
    return NULL;
  }

  // starting the machine

  ret = octopus_register_callback (ctrl->octopus, controller_msg_cbk);
  if (ret) {
    printf ("error registering callback\n");
    // octopus_destroy (ctrl->octopus); @TODO
    free (ctrl);
    return NULL;
  }

  printf ("[INIT] octopus running - starting the server %p\n", controller_receive);

  ctrl->server = ws_server_init ("lo",
                                 2015,
                                 controller_connected,
                                 controller_receive,
                                 controller_close,
                                 ctrl);

  if (!ctrl->server) {
    printf ("error starting ws server \n");
    return NULL;
  }

  ws_server_start (ctrl->server);

  //creating the global loop
  pthread_create (&ctrl->job,
                  NULL,
                  controller_job,
                  ctrl);
  return ctrl;
}
