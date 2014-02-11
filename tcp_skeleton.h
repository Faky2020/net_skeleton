// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// This library is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this library under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this library under a commercial
// license, as set out in <http://cesanta.com/>.

#ifndef TS_SKELETON_HEADER_INCLUDED
#define TS_SKELETON_HEADER_INCLUDED

#define TS_SKELETON_VERSION "1.0"

#include <time.h>      // required for time_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct iobuf {
  char *buf;
  int len;
  int size;
};

void iobuf_init(struct iobuf *, int initial_size);
void iobuf_free(struct iobuf *);
int iobuf_append(struct iobuf *, const void *data, int data_size);
int iobuf_appendf(struct iobuf *, const char *fmt, ...);
void iobuf_remove(struct iobuf *, int data_size);

struct ts_connection {
  struct ts_connection *prev, *next;
  struct ts_server *server;
  void *connection_data;
  time_t last_io_time;
  int sock;
  struct iobuf recv_iobuf;
  struct iobuf send_iobuf;
  void *ssl;
  unsigned int flags;
#define TSF_CLOSE             1
#define TSF_HOLD              2
#define TSF_SSL_HANDS_SHAKEN  4
};

struct ts_server {
  void *server_data;
  int listening_sock;
  struct ts_connection *active_connections;
  void *ssl_ctx;
};

enum ts_event {TS_POLL, TS_ACCEPT, TS_CONNECT, TS_RECV, TS_SEND, TS_CLOSE};
typedef int (*ts_callback_t)(struct ts_connection *, enum ts_event, void *);

void ts_server_init(struct ts_server *, void *server_data);
void ts_server_free(struct ts_server *);
int ts_server_poll(struct ts_server *, int milli, ts_callback_t);
int ts_open_listening_sock(const char *bind_addr);
int ts_connect(struct ts_server *server, const char *host, int port, int ssl);

int ts_send(struct ts_connection *, const void *buf, int len);
int ts_printf(struct ts_connection *, const void *buf, int len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TS_SKELETON_HEADER_INCLUDED