

#include <stdio.h>
#include <stdlib.h>

#include "controller.h"


int32_t main (int argc, char **argv)
{
  struct s_controller_context *ctrl;

  printf ("[INF] Staring...\n");

  ctrl = controller_init ();

  if (!ctrl) {
    printf ("[ERROR] error creating a controller\n");
    return -1;
  }

  printf ("[INF] Started\n");

  //testing loop
  while (1) {

    sleep (5);
  }

  return 0;
}
