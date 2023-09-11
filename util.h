#ifndef _UTIL_H_
#define _UTIL_H_

#include "def.h"

//печатаем дебаг
void dbg(const char * format, ...);
//поиск в указанной области памяти p, блока памяти s
char * memsearch(const char * p, unsigned long plen, const char * s, unsigned long slen);
//парсим url
bool parse_url(char * url, char * sheme, char * host, char * path, unsigned short * port);
//из addr->s_addr т.е. www.ya.ru->u_int
bool get_s_addr( char *addr, unsigned long *ps_addr);
//подключиться к удаленному хосту с созданием сокета
SOCKET socket_create_and_connect(unsigned long ip, unsigned short port);
//создаем сокет и биндим
SOCKET socket_create_and_bind(unsigned short port);
//создаем сокет и биндим и листинем
SOCKET socket_create_and_bind_and_listen(unsigned short port);
//отправка данных по tcp
int tcp_write(SOCKET sock, char * data, int data_size);
//установка timeval в миллисекунды
void set_timeval_ms(timeval * ptimeval, int timeout_ms);
//чтение данных с сокета
int tcp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms);
//иныормация о сокете
bool socket_get_ip_and_port(SOCKET sock, unsigned long * ip, unsigned short * port);
//отправка дпнных по udp
int udp_write(SOCKET sock, char * data, int data_size, unsigned long ip, unsigned short port);
//чтение с udp cjrtnf
int udp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms, unsigned long * from_addr, unsigned short * from_port);
//создать udp сокет и забиндить
SOCKET udp_socket_create_and_bind(unsigned short port);
//тримим строку
char * trim(char * str);
//вправо сдвиг
char * strright(char * str, char * substr);
//акцепт на сокет
SOCKET tcp_accept(SOCKET listener, int timeout_ms);

#endif
