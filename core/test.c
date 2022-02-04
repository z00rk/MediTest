
/*
 * 
 *
 * The computer program contained herein contains proprietary
 * information which is the property of Sagemcom Broadband.
 * The program may be used and/or copied only with the written
 * permission of Sagemcom Broadband or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */


#include <stdio.h>
#include <stdlib.h>

#include "octopus.h"
#include "commander.h"
#include "messenger.h"

int32_t event_cbk (struct s_message_event *p_msg)
{
  printf ("received an eveent %d\n", p_msg->event);
  return 0;
}

int32_t main (int argc, char **argv)
{
  int32_t ret = 0;

  struct s_octopus_context *context;

  context = octopus_init ();

  ret = message_register_callback (context->message_ctx, event_cbk);

  if (ret < 0) {
    printf (" error registering callback\n");
    return -1;
  }

  octopus_start (context);

  while (context->state == OCTOPUS_STATE_RUNNING) {
    sleep (5);
    printf ("sending command 5\n");
    command_send (context->command_ctx, 5);

    sleep (10);
    printf ("sending command 0\n");
    command_send (context->command_ctx, 0);

    sleep (1);
    printf ("sending COMMAND_LAST\n");
    command_send (context->command_ctx, COMMAND_LAST);
  }
  return 0;
}
