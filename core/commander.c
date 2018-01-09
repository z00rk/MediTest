

#include <stdio.h>
#include <stdlib.h>

#include "commander.h"
#include "messenger.h"
#include "octopus.h"
/**
 * This is a starting point for command handling
 * This is responsible for sending commands to the FSM
 * and executing commands when the FSM needs it
 **/

struct s_commander_context *command_init (void *data)
{
  struct s_commander_context *p_context;

  p_context = (struct s_commander_context *) calloc (1, sizeof (struct s_commander_context));
  p_context->p_cmd = (struct s_command *) calloc (1, sizeof (struct s_commander_context));

  pthread_mutex_init (&p_context->cmd_lock, NULL);

  p_context->user_data = data;
  return p_context;
}

int32_t command_send (struct s_commander_context *context,
                      enum e_command p_cmd)
{
  struct s_command *cmd;

  printf ("received a command %d\n", p_cmd);

  if (!context) {
    printf ("bad commander context\n");
    return -1;
  }

  if (!cmd) {
    printf ("bad command to send\n");
    return -1;
  }

  cmd = (struct s_command *) calloc (1, sizeof (struct s_command));
  cmd->cmd = p_cmd;
  cmd->next = NULL;

  pthread_mutex_lock (&context->cmd_lock);
  context->i_cmd_nb++;
  cmd->next = context->p_cmd;
  context->p_cmd = cmd;
  pthread_mutex_unlock (&context->cmd_lock);

  return 0;
}

int32_t command_exec (struct s_command *p_cmd, void *user)
{
  int32_t ret = 0;


  if (!p_cmd) {
    ret = 0;
  } else {
    printf ("executing command \n");
    switch (p_cmd->cmd) {
      case COMMAND_LAST:
        printf ("this is just for test purpose\n");
        struct s_octopus_context *ctx = (struct s_octopus_context *) user;

        message_send (ctx->message_ctx, MESSAGE_EVENT_LAST);
        break;
      default:
        printf ("unhandled command\n");
        ret = -1;
    }
  }

  return ret;
}
