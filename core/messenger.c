

#include <stdlib.h>
#include <stdio.h>

#include "messenger.h"



struct s_message_context *message_init (void *data)
{
  struct s_message_context *p_context;

  p_context = (struct s_message_context *) calloc (1, sizeof (struct s_message_context));

  pthread_mutex_init (&p_context->lock, NULL);

  p_context->user_data = data;

  return p_context;
}

int32_t message_register_callback (struct s_message_context *context,
                                   void* func)
{
  if (!context) {
    printf ("bad message context\n");
    return -1;
  }

  if (!func) {
    printf ("bad function callback\n");
    context->notify_cbk = NULL;
  } else {
    context->notify_cbk = func;
  }

  return 0;
}

int32_t message_send (struct s_message_context *context,
                      enum e_event event)
{
  struct s_message_event *event_list;
  struct s_message_event *p_event;

  if (!context) {
    printf ("error in message context\n");
    return -1;
  }

  event_list = context->event_list;

  p_event = (struct s_message_event *) calloc (1, sizeof (struct s_message_event));
  p_event->event = event;
  p_event->next = NULL;

  printf ("adding an event to the list %p\n", p_event);

  pthread_mutex_lock (&context->lock);
  p_event->next = context->event_list;
  context->event_list = p_event;
  context->i_msg_nb ++;
  pthread_mutex_unlock (&context->lock);

  return 0;
}

int32_t message_notify (struct s_message_context *context,
                        struct s_message_event *p_event,
                        void *user)
{
  if (!context) {
    printf ("error in messaging context\n");
    return -1;
  }

  if (!context->notify_cbk) {
    printf ("error in messaging callback\n");
    return -1;
  }

  if (p_event) {
    printf ("notifying an event %p\n", p_event);
    if (context->notify_cbk) {
      context->notify_cbk (p_event, user);
    }
  }

  return 0;
}
