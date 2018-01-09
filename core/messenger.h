


#ifndef _MESSENGER_H_
#define _MESSENGER_H_

enum e_event {
  MESSAGE_EVENT_LAST
};

struct s_message_event {
  enum e_event event;
  struct s_message_event *next;
};

struct s_message_context {
  pthread_mutex_t lock;
  struct s_message_event *event_list;
  int32_t i_msg_nb;
  int32_t (*notify_cbk)(struct s_message_event *, void *);
  void *user_data;
};


struct s_message_context *message_init (void *);
int32_t message_send (struct s_message_context *, enum e_event);
struct s_message_event *message_event_create (enum e_event);
#endif /** _MESSENGER_H_ **/
