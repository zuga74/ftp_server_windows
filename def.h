#ifndef _DEF_H_
#define _DEF_H_



#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio>

#define THREAD_HANDLE HANDLE
#define IS_VALID_SOCKET(s) (s != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define SLEEP(ms) Sleep(ms)
#define THREAD_FUNCTION(name) DWORD WINAPI name(LPVOID lpParam)
#define CREATE_THREAD(id, fun, par)   id = CreateThread(NULL, 0, fun, (LPVOID)par, 0, NULL); CloseHandle(id)

#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#define THREAD_HANDLE pthread_t
#define SOCKET int
#define INVALID_SOCKET  (int)(~0)
#define SOCKET_ERROR            (-1)
#define MAX_PATH 255
#define SLEEP(ms) usleep(ms * 1000)
#define IS_VALID_SOCKET(s) (s >= 0)
#define CLOSESOCKET(s) close(s)
#define THREAD_FUNCTION(name) void * name(void * lpParam)
#define CREATE_THREAD(id, fun, par)  pthread_create(&id, NULL, fun, (void *)par);  pthread_detach(id)
#define SOCKADDR struct sockaddr

#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <algorithm>
#include <string>
#include <time.h>

#define IPTOS(x) inet_ntoa(*(struct in_addr *)&x)

#define MAX_PACKET_SIZE 1600

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#endif
