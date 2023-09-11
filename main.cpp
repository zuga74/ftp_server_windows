#include <stdio.h>
#include <string.h>
#include "util.h"
#include "ftpserv.h"
#include "ftpconst.h"
#include "ftpfunc.h"
#include "ftpconf.h"



//клиенты
static ftp_session_t ftp_sessions[FD_SETSIZE];
static int ftp_sessions_cnt = 0;


void ftp_restart(ftp_session_t * s, int sec)
{
}

void ftp_cmd_out(ftp_session_t * s, const char * fmt, ...)
{
  char buf[1024];
  va_list arg;

  va_start(arg, fmt);
  vsnprintf(buf, sizeof(buf), fmt, arg);
  va_end(arg);

  strcat(buf, "\r\n");
  tcp_write(s->con_cmd_id, buf, strlen(buf));
  printf(buf);
}

int ftp_data_out(ftp_session_t * s, const char * data, int data_len)
{
  return tcp_write(s->con_data_id, (char *)data, data_len);
}

int ftp_data_in(ftp_session_t * s, char * buf, int buf_size)
{
   return tcp_read(s->con_data_id, buf, buf_size, 10000);
}

bool ftp_connect(ftp_session_t * s)
{
  SOCKET sock;

  if (s->passive) {
    sock = tcp_accept(s->con_data_id, 10000);
    CLOSESOCKET(s->con_data_id);
    s->con_data_id = sock;
  } else {
    CLOSESOCKET(s->con_data_id);
    s->con_data_id = socket_create_and_connect(htonl(s->host), s->port);
  }
  return s->con_data_id > 0;
}

void ftp_disconnect(ftp_session_t * s)
{
  CLOSESOCKET(s->con_data_id);
}

bool ftp_pasv(ftp_session_t * ftp_session)
{
  unsigned long ip;
  unsigned short port;
  char buf[512];
  struct hostent *host;

  CLOSESOCKET(ftp_session->con_data_id);

  ftp_session->con_data_id = socket_create_and_bind_and_listen(0);
  if (ftp_session->con_data_id < 1) return false;

  socket_get_ip_and_port(ftp_session->con_data_id, &ip, &port);

  gethostname(buf, sizeof(buf));
  host = gethostbyname(buf);
  if (host) ftp_session->host = *(u_long *)host->h_addr_list[0];
  else ftp_session->host = inet_addr("127.0.0.1");
  ftp_session->port = port;
  return true;
}


void ftp_on_accept(int index)
{
  do_ACCEPT(&ftp_sessions[index], NULL);
}

void ftp_close_session(int index)
{
  CLOSESOCKET(ftp_sessions[index].con_cmd_id);
  CLOSESOCKET(ftp_sessions[index].con_data_id);
  if (ftp_sessions_cnt > 1) {
    ftp_sessions[index] = ftp_sessions[ftp_sessions_cnt - 1];
  }
  if (ftp_sessions_cnt) ftp_sessions_cnt--;
}

void ftp_quit(ftp_session_t * s)
{
  int i;

  for (i = 0; i <FD_SETSIZE; ++i) {
    if (s == &ftp_sessions[i]) {
      ftp_close_session(i);
      break;
    }
  }
}

void ftp_on_recv_cmd(int index)
{
  int rc;
  char buf[MAX_PACKET_SIZE];

  rc = recv(ftp_sessions[index].con_cmd_id, buf, sizeof(buf), 0);
  if (rc <= 0 ) {
    ftp_close_session(index);
    return;
  }

  buf[rc] = '\0';


  printf(buf);

  ftp_cmd_in(&ftp_sessions[index], buf);
}

int main(int argc, char* argv[])
{
  SOCKET listener, sock;
  fd_set readset;
  TIMEVAL timeval;
  int rc, i;

#ifdef _WIN32
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif

  listener = socket_create_and_bind_and_listen(FTP_SERVER_PORT);
  if (listener == SOCKET_ERROR) return 0;

  timeval.tv_sec = 1;
  timeval.tv_usec = 0;

  while (true) {
    FD_ZERO(&readset);
    FD_SET(listener, &readset);

    for (i = 0; i < ftp_sessions_cnt; ++i) {
      FD_SET(ftp_sessions[i].con_cmd_id, &readset);
    }

    rc = select(0, &readset, NULL, NULL, &timeval);
    if (rc < 0) break;
    if (rc == 0) {
       SLEEP(100);
       continue;
    }

    if (FD_ISSET(listener, &readset)) {
      if (ftp_sessions_cnt < FD_SETSIZE) {
        sock = accept(listener, NULL, NULL);
        if (sock != INVALID_SOCKET) {
          ftp_sessions[ftp_sessions_cnt].con_cmd_id = sock;
          strcpy(ftp_sessions[ftp_sessions_cnt].dir, "/");
          ftp_sessions[ftp_sessions_cnt].passive = false;
          ftp_on_accept(ftp_sessions_cnt);
          ftp_sessions_cnt++;
        }
      }
      else printf("Client broken\r\n");
    }

    for (i = 0; i < ftp_sessions_cnt; i++) {
      sock = ftp_sessions[i].con_cmd_id;
      if (!FD_ISSET(sock, &readset)) continue;
      ftp_on_recv_cmd(i);
    }
  }

  for (i = 0; i < ftp_sessions_cnt; ++i) {
    CLOSESOCKET(ftp_sessions[i].con_cmd_id);
    CLOSESOCKET(ftp_sessions[i].con_data_id);
  }

  CLOSESOCKET(listener);

#ifdef _WIN32
  WSACleanup();
#endif
  return 0;
}




