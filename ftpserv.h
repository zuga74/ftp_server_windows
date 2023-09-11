#ifndef ftpservH
#define ftpservH

typedef struct _ftp_session {
  int con_cmd_id; // connection command id
  int con_data_id; // connection data id
  char dir[512];
  int type;
  unsigned long host;
  unsigned short port;
  int curr_command;
  char rename[512];
  bool passive;
  int stru;
  int mode;
} ftp_session_t;

typedef void (*FTP_HANDLER)(ftp_session_t *s, char *param);


//входящие данные для комманд, data - данные
void ftp_cmd_in(ftp_session_t * s, char * data);
//должна быть определена!!! исходящие данные для комманд
void ftp_cmd_out(ftp_session_t * s, const char * fmt, ...);

//Должна быть определена!!! забирает входящие данные для данных
int ftp_data_in(ftp_session_t * s, char * buf, int buf_size);
//должна быть определена!!! исходящие данные для данных
int ftp_data_out(ftp_session_t * s, const char * data, int data_len);


//должна быть определена!!! должна удалить сессию
void ftp_quit(ftp_session_t * s);

//должна быть определена!!! должна создать пассивный сокет данных
bool ftp_pasv(ftp_session_t * s);

//должна быть определена!!! должна соединиться с сокетом данных
bool ftp_connect(ftp_session_t * s);

//должна быть определена!!! должна закрыть сокет данных
void ftp_disconnect(ftp_session_t * s);

//должна быть определена!!! должна перезагрузить сессию через sec
void ftp_restart(ftp_session_t * s, int sec);

#endif
