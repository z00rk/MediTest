


#ifndef _COMMANDER_H_
#define _COMMANDER_H_

// command list to be filled
enum e_command {
  COMMAND_LAST
};

// command struct to be filled
struct s_command {
  enum e_command cmd;
  struct s_command *next;
};

// command context to be saved
struct s_commander_context {
  pthread_mutex_t cmd_lock;
  int32_t i_cmd_nb;
  struct s_command *p_cmd;
  void *user_data;
};

struct s_commander_context *command_init (void *);
int32_t command_exec (struct s_command *, void *);
#endif /** _COMMANDER_H_ **/
