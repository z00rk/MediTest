

#ifndef _WEBSOCKET_H_
#define _WEBSOCKET_H_

#include <pthread.h>
#include <libwebsockets.h>

#define WS_SERVER_BUFFER_SIZE (100 * 1024)


struct s_ws_reply {
  struct s_ws_reply *n;
  char *str;
  char *cptr;
  size_t csize;
};

struct s_ws_client {
  struct s_ws_client *p;
  struct s_ws_client *n;
  struct libwebsocket *socket;
  struct s_ws_reply *replies;
  struct s_ws_reply *replies_q;
  void *client_data;
};

struct s_ws_server {
  struct libwebsocket_context *ws_context;
  struct lws_context_creation_info ws_context_info;
  int32_t (*server_established)(void *js, void *client, void *user);
  int32_t (*server_receive)(void *js, int len, char *message, void *user);
  int32_t (*server_closed)(void *js, void *client, void *user);
  pthread_mutex_t lock;
  struct s_ws_client *client_list;
  void *user_data;
};

int32_t ws_server_callback (struct libwebsocket_context *context,
                            struct libwebsocket *socket,
                            enum libwebsocket_callback_reasons reason,
                            void *data,
                            void *in,
                            size_t len);

struct s_ws_server *ws_server_init (char *iface,
                                    int port,
                                    int32_t (*established)(void *, void *, void *),
                                    int32_t (*received)(void *, int, char *, void *),
                                    int32_t (*closed)(void *, void *, void *),
                                    void *data);
int32_t ws_server_run (struct s_ws_server *);
int32_t ws_server_start (struct s_ws_server *);
int32_t ws_server_stop (struct s_ws_server *);

#endif /** _WEBSOCKET_H_ **/
