#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


char * strright(char * str, char * substr)
{
  char * ptr;

  ptr = strstr(str, substr);
  return ptr ? ptr + strlen(substr) : NULL;
}

void dbg(const char * format, ...)
{
  va_list argptr;

  va_start(argptr, format);
  vprintf(format, argptr);
  va_end(argptr);
}


char * memsearch(const char * p, unsigned long plen, const char * s, unsigned long slen)
{
  char * cp = (char *)p;
  char * result = NULL;

  if (slen == 0) return NULL;

  while ( (cp = (char *)memchr(cp, s[0], plen + (unsigned long)(p - cp) )) != NULL )
  {
    if ( cp + slen > p + plen )
      break;
    else if ( memcmp(cp, s, slen) == 0 )
    {
      result = cp;
      break;
    }
    else cp++;
  }
  return result;
}


bool parse_url(char * url, char * sheme, char * host, char * path, unsigned short * port)
{
  char * pch = url;
  char * pche, * pchb, * pcht;
  int l;
  int len = l = strlen(url);

  sheme[0] = '\0';
  host[0] = '\0';
  path[0] = '\0';
  *port = 0;

  if ( NULL != (pch = memsearch(url, l, "://", 3)) )
  {
     memcpy(sheme, url, pch - url);
     sheme[pch - url] = 0;
     pch += 3;
     l = len - (pch - url);
  } else pch = url;

  if ( NULL == (pche = memsearch(pch, l, "/", 1)) )
    pche = pch + l;

  pchb = pche;
  if ( NULL != (pcht = memsearch(pch, pche - pch, ":", 1)) )
  {
    *port = atoi(pcht + 1);
    if (*port == 0) {
       if ( NULL != (pcht = memsearch(pcht + 1, pche - pch, ":", 1)) ) {
         *port = atoi(pcht + 1);
         pche = pcht;
       }
    }
    else pche = pcht;
  }
  else *port = 0;

  memcpy(host, pch, pche - pch);
  host[pche - pch] = 0;
   pch = pchb;
  l = len - (pch - url);

  memcpy(path, pch, l);
  path[l] = 0;
  if (strlen(path) == 0) { path[0] = '/'; path[1] = '\0'; }

  return true;
}


bool get_s_addr(char *addr, unsigned long *ps_addr )
{
  struct hostent *hp;



  if (strcmp(addr, "localhost") == 0)
  {
    *ps_addr = inet_addr("127.0.0.1");
    return true;
  }

  if ((*ps_addr = inet_addr(addr)) != INADDR_NONE) return true;


  if((hp = gethostbyname(addr)) != NULL)
  {
     *ps_addr = (( struct in_addr * )hp->h_addr)->s_addr;
     return true;
  }


  return false;
}


SOCKET socket_create_and_connect(unsigned long ip, unsigned short port)
{
  SOCKET sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if ( !IS_VALID_SOCKET(sock) ) return SOCKET_ERROR;

  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = ip;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (SOCKET_ERROR == connect(sock, (sockaddr*)&addr, sizeof(addr)) ) //Конектимся к требуемому адресу
  {
    CLOSESOCKET(sock);
    return SOCKET_ERROR;
  }

  return sock;
}

SOCKET socket_create_and_bind(unsigned short port)
{
  SOCKET sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if ( !IS_VALID_SOCKET(sock) ) return SOCKET_ERROR;

  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (SOCKET_ERROR == bind(sock, (sockaddr*)&addr, sizeof(addr)))
  {
    CLOSESOCKET(sock);
    return SOCKET_ERROR;
  }

  return sock;
}

SOCKET socket_create_and_bind_and_listen(unsigned short port)
{
  SOCKET sock;

  sock = socket_create_and_bind(port);
  if (SOCKET_ERROR == sock) return SOCKET_ERROR;

  if (SOCKET_ERROR == listen(sock, SOMAXCONN)) {
    CLOSESOCKET(sock);
    return SOCKET_ERROR;
  }

  return sock;
}

int tcp_write(SOCKET sock, char * data, int data_size)
{
    int rc, sz =  data_size;

    while (sz > 0)
    {
        rc = send(sock, data, sz, 0);

        if (rc <= 0)  break;

        data  += rc;
        sz -= rc;
    }


    return data_size - sz;
}

int udp_write(SOCKET sock, char * data, int data_size, unsigned long ip, unsigned short port)
{
    int rc, sz = data_size;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip;

    while (sz > 0)
    {
        rc = sendto(sock, data, sz, 0, (SOCKADDR *)&addr, sizeof(addr));

        if (rc <= 0)  break;

        data  += rc;
        sz -= rc;
    }


    return data_size - sz;
}

void set_timeval_ms(timeval * ptimeval, int timeout_ms)
{
  ptimeval->tv_sec = timeout_ms / 1000;
  ptimeval->tv_usec = (timeout_ms - ptimeval->tv_sec * 1000) * 1000;
}

int tcp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms)
{
  fd_set readset;
  timeval timeout;
  char * ptr = buf;
  int rc, sz = buf_size;

  set_timeval_ms(&timeout, timeout_ms);

  while (sz > 0)
  {
    FD_ZERO(&readset);
    FD_SET(sock, &readset);

    if (select(FD_SETSIZE, &readset, NULL, NULL, &timeout) == SOCKET_ERROR) break;

    if (!FD_ISSET(sock, &readset)) break;

    rc = recv(sock, ptr, sz, 0);
    if (rc <= 0 ) return -1;

    ptr += rc;
    sz -= rc;

    set_timeval_ms(&timeout, MAX(10, timeout_ms / 10));
  }

  return ptr - buf;
}

SOCKET tcp_accept(SOCKET listener, int timeout_ms)
{
  fd_set readset;
  timeval timeout;
  int rc;

  set_timeval_ms(&timeout, timeout_ms);

  FD_ZERO(&readset);
  FD_SET(listener, &readset);

  rc = select(0, &readset, NULL, NULL, &timeout);

  if (rc < 1) return rc;

  return accept(listener, NULL, NULL);
}


int udp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms, unsigned long * from_addr, unsigned short * from_port)
{
  fd_set readset;
  timeval timeout;
  struct sockaddr_in addr;
  int rc;

  set_timeval_ms(&timeout, timeout_ms);

  FD_ZERO(&readset);
  FD_SET(sock, &readset);

  if (select(FD_SETSIZE, &readset, NULL, NULL, &timeout) == SOCKET_ERROR) return 0;

  if (!FD_ISSET(sock, &readset)) return 0;

  socklen_t len = sizeof(addr);
  rc = recvfrom(sock, buf, buf_size, 0, (struct sockaddr *)&addr, &len);
  if (rc <= 0 ) return -1;

  *from_addr = addr.sin_addr.s_addr;
  *from_port = ntohs(addr.sin_port);

  return rc;
}

bool socket_get_ip_and_port(SOCKET sock, unsigned long * ip, unsigned short * port)
{
  struct sockaddr_in addr;

  if ( !IS_VALID_SOCKET(sock) ) return false;

  socklen_t len = sizeof(addr);
  if (getsockname(sock, (struct sockaddr *)&addr, &len) == SOCKET_ERROR) return false;
  *ip = addr.sin_addr.s_addr;
  *port = ntohs(addr.sin_port);

  return true;
}

SOCKET udp_socket_create_and_bind(unsigned short port)
{
  SOCKET sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  if ( !IS_VALID_SOCKET(sock) ) return SOCKET_ERROR;


  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (SOCKET_ERROR == bind(sock, (struct sockaddr *)&addr, sizeof(addr))) {
    CLOSESOCKET(sock);
    return SOCKET_ERROR;
  }

  return sock;
}

char * trim(char * str)
{
  char * res = NULL;
  int i;

  for (i = 0; i < (int)strlen(str); ++i) {
    if ( str[i] > ' ') {
      res = str + i;
      break;
    }
  }

  if (res == NULL) return res;

  int len = strlen(str) - (res - str);

  if (len <= 0) return NULL;


  for (i = len - 1 ; i > -1; --i) {
    if ( str[i] > ' ') {
      len = str + i  + 1 - res;
      str[len] = '\0';
      break;
    }
  }

  return res;
}


