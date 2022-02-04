

#include <stdio.h>
#include <stdlib.h>
#include "websocket.h"

int32_t est_cbk (void *js, void *client)
{
  printf ("connection established \n");
  return 0;
}

int32_t rec_cbk (void *js, int len, char *msg)
{
  printf (" received message (%s) of len %d\n", msg, len);
  return 0;
}

int32_t close_cbk (void *js, void *client)
{
  printf ("connection closed \n");
  return 0;
}

int32_t main (int argc, char **argv)
{
  struct s_ws_server *server;

  server = ws_server_init ("127.0.0.1",
                           6666,
                           est_cbk,
                           rec_cbk,
                           close_cbk);

  if (!server) {
    printf ("error init websocket server\n");
    return -1;
  }

  if (ws_server_start (server) != 0) {
    printf ("error starting websocket server \n");
    return -1;
  }

  while (1) {
    ws_server_run (server);
    usleep (100000);
  }

  return 0;
}
