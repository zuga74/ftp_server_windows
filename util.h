#ifndef _UTIL_H_
#define _UTIL_H_

#include "def.h"

//�������� �����
void dbg(const char * format, ...);
//����� � ��������� ������� ������ p, ����� ������ s
char * memsearch(const char * p, unsigned long plen, const char * s, unsigned long slen);
//������ url
bool parse_url(char * url, char * sheme, char * host, char * path, unsigned short * port);
//�� addr->s_addr �.�. www.ya.ru->u_int
bool get_s_addr( char *addr, unsigned long *ps_addr);
//������������ � ���������� ����� � ��������� ������
SOCKET socket_create_and_connect(unsigned long ip, unsigned short port);
//������� ����� � ������
SOCKET socket_create_and_bind(unsigned short port);
//������� ����� � ������ � ��������
SOCKET socket_create_and_bind_and_listen(unsigned short port);
//�������� ������ �� tcp
int tcp_write(SOCKET sock, char * data, int data_size);
//��������� timeval � ������������
void set_timeval_ms(timeval * ptimeval, int timeout_ms);
//������ ������ � ������
int tcp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms);
//���������� � ������
bool socket_get_ip_and_port(SOCKET sock, unsigned long * ip, unsigned short * port);
//�������� ������ �� udp
int udp_write(SOCKET sock, char * data, int data_size, unsigned long ip, unsigned short port);
//������ � udp cjrtnf
int udp_read(SOCKET sock, char * buf, int buf_size, int timeout_ms, unsigned long * from_addr, unsigned short * from_port);
//������� udp ����� � ���������
SOCKET udp_socket_create_and_bind(unsigned short port);
//������ ������
char * trim(char * str);
//������ �����
char * strright(char * str, char * substr);
//������ �� �����
SOCKET tcp_accept(SOCKET listener, int timeout_ms);

#endif
