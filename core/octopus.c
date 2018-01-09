
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "octopus.h"

int32_t octopus_register_callback (struct s_octopus_context *context, void *func)
{
  if (!context || !func) {
    printf ("bad context or callback\n");
    return -1;
  }

  return message_register_callback (context->message_ctx, func);
}

int32_t octopus_destroy (struct s_octopus_context *context)
{
  if (!context) {
    printf ("unkown context \n");
    return -1;
  }

  free (context);
  return 0;
}

struct s_octopus_context *octopus_init (void *data)
{
  struct s_octopus_context *context;
  struct s_message_context *msg_ctx;
  struct s_commander_context *cmd_ctx;

  //strating the FSM context
  context = (struct s_octopus_context *) calloc (1, sizeof (struct s_octopus_context));

  context->state = OCTOPUS_STATE_INIT;

  context->message_ctx = message_init (data);
  context->command_ctx = command_init (data);

  context->user_data = data;

  return context;
}


int32_t octopus_start (struct s_octopus_context *context)
{
  if (!context) {
    printf ("bad FSM context\n");
    return -1;
  }

  printf ("Launching the main thread\n");

  context->state = OCTOPUS_STATE_RUNNING;

  if (pthread_create (&context->job,
                      NULL,
                      octopus_job,
                      context) != 0) {
    printf ("error starting main thread\n");
    return -1;
  }

  return 0;
}

void *octopus_job (void *arg)
{
  struct timeval timeout;
  struct s_octopus_context *context = (struct s_octopus_context *) arg;
  struct s_commander_context *cmd_ctx = context->command_ctx;
  struct s_message_context *msg_ctx = context->message_ctx;
  struct s_command *command;
  struct s_message_event *message;

  printf ("entering the job\n");

  context->state = OCTOPUS_STATE_RUNNING;

  for (;;) {

    timeout.tv_sec  = 0;
    timeout.tv_usec = 10000;

    pthread_mutex_lock (&cmd_ctx->cmd_lock);

    if (cmd_ctx->p_cmd) {
      command = cmd_ctx->p_cmd;
      cmd_ctx->p_cmd = command->next;
      cmd_ctx->i_cmd_nb--;
    }

    pthread_mutex_unlock (&cmd_ctx->cmd_lock);

    if (command_exec (command, context) != 0) {
      printf ("error exec command\n");
    }

    if (command) {
      free (command);
      command = NULL;
    }

    pthread_mutex_lock (&msg_ctx->lock);

    if (msg_ctx->event_list) {
      message = msg_ctx->event_list;
      msg_ctx->event_list = message->next;
      msg_ctx->i_msg_nb--;
    }

    pthread_mutex_unlock (&msg_ctx->lock);

    if (message_notify (msg_ctx, message, context) != 0) {
      printf ("error process event\n");
    }

    if (message) {
      free (message);
      message = NULL;
    }

    //select (0, NULL, NULL, NULL, &timeout);
    usleep (100000);
  }

  printf ("exiting the loop\n");

  return;
}
