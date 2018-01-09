/**
 * Web socket server
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "websocket.h"

int32_t ws_server_run (struct s_ws_server *server)
{
  struct s_ws_client *client;

  libwebsocket_service(server->ws_context, 0);

  pthread_mutex_lock(&server->lock);

  client = server->client_list;
  while (client != NULL) {
    if (client->replies != NULL)
      libwebsocket_callback_on_writable (server->ws_context, client->socket);
    client = client->n;
  }
  pthread_mutex_unlock(&server->lock);

  return 0;
}

struct s_ws_server *ws_server_init (char *iface,
                                    int port,
                                    int32_t (*established)(void *js, void *client, void *data),
                                    int32_t (*receive)(void *js, int len, char *in, void *data),
                                    int32_t (*closed)(void *js, void *client, void *data),
                                    void *user)
{
  struct s_ws_server *server = NULL;
  pthread_mutexattr_t attributes;

  server = (struct s_ws_server *) malloc (sizeof (struct s_ws_server));
  memset (server, 0x00, sizeof (struct s_ws_server));

  if (iface) {
    server->ws_context_info.iface = strdup (iface);
  }

  server->ws_context_info.port = port;
  server->ws_context_info.protocols = calloc (2, sizeof (struct libwebsocket_protocols));
  server->ws_context_info.protocols[0].name = "lws-bidirectional-protocol";
  server->ws_context_info.protocols[0].callback = ws_server_callback;
  server->ws_context_info.protocols[0].per_session_data_size = sizeof (struct s_ws_client);
  server->ws_context_info.protocols[0].rx_buffer_size = WS_SERVER_BUFFER_SIZE;
  server->ws_context_info.gid       = -1;
  server->ws_context_info.uid       = -1;
//  server->ws_context_info.options   = 4|6;
  server->ws_context_info.user      = server;

  server->server_established = established;
  server->server_receive     = receive;
  server->server_closed      = closed;
  server->user_data = user;

  pthread_mutexattr_init (&attributes);
  pthread_mutexattr_settype (&attributes, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init (&server->lock, &attributes);

  server->user_data = user;
  return server;
}

int32_t ws_libwebsocket_init(struct s_ws_server *server)
{
  struct libwebsocket_context *ctx;

  if ((ctx = libwebsocket_create_context (&server->ws_context_info)) == NULL) {
    printf ("! libwebsocket_create_context iface '%s' failed\n", server->ws_context_info.iface);
    if (server->ws_context_info.iface != NULL)
      free ((char *)server->ws_context_info.iface);
    free (server->ws_context_info.protocols);
    pthread_mutex_destroy (&server->lock);
    free (server);
    return -1;
  }
  else
    server->ws_context = ctx;
  return 0;
}

int32_t ws_server_start (struct s_ws_server *server)
{
  if (!server) {
    printf ("Error starting server - no init done ?\n");
    return -1;
  } else {
    if (ws_libwebsocket_init (server) < 0) {
      printf ("error starting libwebsocket \n");
      return -1;
    }
  }

  return 0;
}

int32_t ws_server_stop (struct s_ws_server *server)
{
  struct s_ws_client *client = NULL;
  struct s_ws_reply  *reply  = NULL;

  pthread_mutex_lock (&server->lock);
  while (client) {
    while (client->replies) {
      reply = client->replies;
      client->replies = reply->n;
      free (reply);
    }
  }

  if (server->ws_context) {
    libwebsocket_context_destroy (server->ws_context);
  }

  pthread_mutex_unlock (&server->lock);
  pthread_mutex_destroy (&server->lock);

  if (server->ws_context_info.iface) {
    free ((void *) server->ws_context_info.iface);
  }

  free (server->ws_context_info.protocols);
  free (server);

  return 0;
}


/**
 * @TODO : remove all cson dependencies and modify the receive callback signature
**/
int32_t ws_server_callback (struct libwebsocket_context *context,
                            struct libwebsocket *socket,
                            enum libwebsocket_callback_reasons reason,
                            void *data,
                            void *in,
                            size_t len)
{
  struct s_ws_client *client;
  struct s_ws_reply  *reply;
  struct s_ws_server *server = (struct s_ws_server *) libwebsocket_context_user (context);
  size_t w, s;
  int opts = 0;

  switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
      printf ("[WS] Connection established \n");
      memset(client, 0, sizeof (struct s_ws_client));
      client->socket = socket;
      if (server->server_established) {
        server->server_established (server, client, server->user_data);
      }
      pthread_mutex_lock (&server->lock);
      if (server->client_list != NULL) {
        client->n = server->client_list;
        server->client_list->p = client;
      }
      server->client_list = client;
      pthread_mutex_unlock (&server->lock);
      break;
    case LWS_CALLBACK_CLOSED:
      printf ("[WS] Connection closed \n");
      pthread_mutex_lock (&server->lock);
      while(client->replies) {
        reply = client->replies->n;
        free (client->replies->str);
        free (client->replies);
        client->replies = reply;
      }
      if (server->server_closed)
        server->server_closed (server, client, server->user_data);
      if (client == server->client_list) {
        server->client_list = client->n;
        client->p = NULL;
      } else {
        client->p->n = client->n;
        if (client->n)
          client->n->p = client->p;
      }
      pthread_mutex_unlock (&server->lock);
      break;
    case LWS_CALLBACK_RECEIVE:
      printf ("[WS] Message received on socket %s\n", (char *)in);
      if (server->server_receive) {
        printf ("[WS] triggering the receive callback\n");
        server->server_receive (server, len, in, server->user_data);
      }
      break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
      printf ("[WS] Sending message\n");
      pthread_mutex_lock (&server->lock);
      if (client->replies) {
        reply = client->replies;
        s = reply->csize;
        w = libwebsocket_write(socket,
                               (unsigned char *)reply->cptr,
                               s,
                               opts | LWS_WRITE_TEXT);
        if (w < 0) {
          printf ("! LWS_CALLBACK_SERVER_WRITEABLE what !!!\n");
        } else if (w != s) {
          reply->csize -= w;
          reply->cptr += w;
        } else {
          client->replies = reply->n;
          if (client->replies == NULL)
            client->replies_q = NULL;
          free (reply->str);
          free (reply);
        }
        if (client->replies != NULL)
          libwebsocket_callback_on_writable (context, socket);
      }
      pthread_mutex_unlock(&server->lock);
      break;
    default:
      printf ("unhandled case in web socket sever %d\n", reason);
      break;
  }
  return 0;
}

