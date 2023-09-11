#include "ftpfunc.h"
#include "ftpconf.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"


const FTP_HANDLER ftp_handlers[MAX_FTP_COMMANDS] = {
    // Access Control Commands
    do_USER, do_PASS, /*do_ACCT*/ do_NIMP, do_CWD,
    do_CDUP, /*do_SMNT*/ do_NIMP, /*do_REIN*/ do_NIMP,  do_QUIT,
    // Transfer Parameter Commands
    do_PORT, do_PASV, do_TYPE, do_STRU,
    do_MODE,
    // FTP Service Commands
    do_RETR, do_STOR, /*do_STOU*/ do_NIMP, /*do_APPE*/ do_NIMP,
    /*do_ALLO*/ do_NIMP, do_REST, do_RNFR, do_RNTO,
    /*do_ABOR*/ do_NIMP, do_DELE, do_RMD, do_MKD,
    do_PWD,  do_LIST, /*do_NLST*/ do_LIST, /*do_SITE*/do_NIMP,
    do_SYST, /*do_STAT*/ do_NIMP, do_HELP, do_NOOP,
    // From RFC 1123
    /*do_XCWD*/ do_CWD, /*do_XCUP*/do_CDUP, /*do_XRMD*/ do_DELE, /*do_XMKD*/ do_MKD,
    /*do_XPWD*/ do_PWD
};



bool is_file_exists(const char *dir)
{
    int code = GetFileAttributes(dir);
    return (code != -1) && ((code & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

bool is_dir_exists(const char *dir)
{
    int code = GetFileAttributes(dir);
    return (code != -1) && ((code & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

//сообщение при соединении клиента
void do_ACCEPT(ftp_session_t * s, char * param)
{
  ftp_cmd_out(s, ftp_code_220_serv_ready);
}

//закрыть сессию
void do_QUIT(ftp_session_t *s, char *param)
{

  ftp_cmd_out(s, ftp_code_221_bye);
  ftp_quit(s);
}

//не поддерживается
void do_NIMP(ftp_session_t *s, char *param)  // NIMP = not implemented
{
  ftp_cmd_out(s, ftp_code_502_cmd_not_implement);
}

//пользователь
void do_USER(ftp_session_t *s, char *param)
{
  if (strcmp(param, FTP_USER) == 0) ftp_cmd_out(s, ftp_code_331_user_ok);
  else ftp_cmd_out(s, ftp_code_530_user_not_log);
}


//пароль
void do_PASS(ftp_session_t *s, char *param)
{
  if (strcmp(param, FTP_PASS) == 0) ftp_cmd_out(s, ftp_code_230_user_log);
  else ftp_cmd_out(s, ftp_code_530_user_not_log);
}

void get_system_dir(ftp_session_t *s, char *param, char * buf)
{

  strcpy(buf, "c:");

  if (!param) {
    strcat(buf, s->dir);
    return;
  }

  if (param[0] == '/') { //от корня
    strcat(buf, param);
  } else {
    strcat(buf, s->dir);
    if (s->dir[strlen(s->dir) - 1] != '/') strcat(buf, "/");
    strcat(buf, param);
  }
}

//сменить текущую директорию
void do_CWD(ftp_session_t *s, char *param)
{
  char buf[512];


  if (strcmp(param, "/") == 0) {
    strcpy(s->dir, "/");
    ftp_cmd_out(s, ftp_code_200_ok);
    return;
  }

  if (strcmp(param, "..") == 0) {
    do_CDUP(s, param);
    return;
  }

  get_system_dir(s, param, buf);

  if (is_dir_exists(buf)) {
    strcpy(s->dir, buf + 2);
    ftp_cmd_out(s, ftp_code_200_ok);
  } else {
    ftp_cmd_out(s, ftp_code_550_no_file);
  }

}

//текущая директория
void do_PWD(ftp_session_t *s, char *param)
{
  ftp_cmd_out(s, ftp_code_257_dir, s->dir);
}


//вверх от текущей директории
void do_CDUP(ftp_session_t *s, char *param)
{
  int i;

  for (i = strlen(s->dir) - 1; i >= 0; i--) {
     if (s->dir[i] == '/') {
       if (i) s->dir[i] = '\0';
       else strcpy(s->dir, "/");
       ftp_cmd_out(s, ftp_code_200_ok);
       return;
     }
  }
  ftp_cmd_out(s, ftp_code_550_no_file);
}


//порт для пассивного соединения
void do_PORT(ftp_session_t *s, char *param)
{
  char * ptr;

  s->host = 0;
  s->host = atoi(param) << 24;
  ptr = strright(param, ",");
  if (!ptr) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  s->host |= atoi(ptr) << 16;
  ptr = strright(ptr, ",");
  if (!ptr) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  s->host |= atoi(ptr) << 8;
  ptr = strright(ptr, ",");
  if (!ptr) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  s->host |= atoi(ptr);

  s->port = 0;
  ptr = strright(ptr, ",");
  if (!ptr) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  s->port = atoi(ptr) << 8;
  ptr = strright(ptr, ",");
  if (!ptr) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  s->port |= atoi(ptr);

  ftp_cmd_out(s, ftp_code_200_ok);
}

//перейти в пассивный режим
void do_PASV(ftp_session_t *s, char *param)
{
  char buf[512];
  unsigned char * ph, * pp;

  if (!ftp_pasv(s)) {
    ftp_cmd_out(s, ftp_code_421_serv_not_available);
    return;
  }


  ph = (unsigned char *)&s->host;
  pp = (unsigned char *)&s->port;


  ftp_cmd_out(s, ftp_code_257_enter_pasv_mode, ph[0], ph[1], ph[2], ph[3], pp[1], pp[0]);

  s->passive = true;
}

//установить тип файлов
void do_TYPE(ftp_session_t *s, char *param)
{
  int type;

  type = toupper(param[0]);

  if (!IS_TYPE_CODE(type)) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }

  if (type == 'E' || type == 'L' || (type == 'A' && param[1] == ' '))  {
    ftp_cmd_out(s, ftp_code_504_cmd_not_implement_param);
    return;
  }
  s->type = type;
  ftp_cmd_out(s, ftp_code_200_ok);
}

void do_STRU(ftp_session_t *s,char *param)
{
  int stru;

  stru = toupper(*param);
  if (!IS_STRU_CODE(stru)) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }

  if (stru != 'F') {
    ftp_cmd_out(s, ftp_code_504_cmd_not_implement_param);
    return;
  }
  s->stru = stru;
  ftp_cmd_out(s, ftp_code_200_ok);
}

void do_MODE(ftp_session_t *s, char *param)
{
  int mode;

  mode = toupper(*param);
  if (!IS_MODE_CODE(mode)) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }
  if (mode == 'C') {
    ftp_cmd_out(s, ftp_code_504_cmd_not_implement_param);
    return;
  }
  s->mode = mode;
  ftp_cmd_out(s, ftp_code_200_ok);
}

//передать файл клиенту
void do_RETR(ftp_session_t *s, char *param)
{
  char buf[512];
  FILE *fp;
  int read, write;


  get_system_dir(s, param, buf);

  fp = fopen(buf, "rb");
  if (!fp) {
    ftp_cmd_out(s, ftp_code_450_file_unavailable);
    return;
  }

  ftp_cmd_out(s, ftp_code_150_file_ok);

  if (!ftp_connect(s)) {
    ftp_cmd_out(s, ftp_code_425_not_data_connect);
    return;
  }

  while (!feof(fp)) {
    read = fread(buf, 1, sizeof(buf), fp);
    if (ferror(fp)) {
      ftp_disconnect(s);
      fclose(fp);
      ftp_cmd_out(s, ftp_code_451_abort_local);
      return;
    }
    write = ftp_data_out(s, buf, read);
    if (read != write) {
      ftp_disconnect(s);
      fclose(fp);
      ftp_cmd_out(s, ftp_code_451_abort_local);
      return;
    }
  }
  fclose(fp);
  ftp_disconnect(s);

  ftp_cmd_out(s, ftp_code_226_trans_complete);
}

//прuнять файл от клиента
void do_STOR(ftp_session_t *s, char *param)
{
  char buf[512];
  FILE *fp;
  int read, write;

  get_system_dir(s, param, buf);

  fp = fopen(buf, "wb");
  if (!fp) {
    ftp_cmd_out(s, ftp_code_450_file_unavailable);
    return;
  }

  ftp_cmd_out(s, ftp_code_150_file_ok);

  if (!ftp_connect(s)) {
    ftp_cmd_out(s, ftp_code_425_not_data_connect);
    return;
  }


  for (;;) {
    read = ftp_data_in(s, buf, sizeof(buf));
    //read = recv(s->con_data_id, buf, sizeof(buf), 0);
    if (!read) break;
    if (read < 0) {
      fclose(fp);
      ftp_disconnect(s);
      ftp_cmd_out(s, ftp_code_426_trans_abort);
      return;
    }
    write = fwrite(buf, 1, read, fp);
    if (read != write) {
      fclose(fp);
      ftp_disconnect(s);
      ftp_cmd_out(s, ftp_code_426_trans_abort);
      return;
    }
  }
  fclose(fp);
  ftp_disconnect(s);
  ftp_cmd_out(s, ftp_code_226_trans_complete);
}

//рестарт
void do_REST(ftp_session_t *s, char *param)
{
  ftp_cmd_out(s, ftp_code_350_restarting, atoi(param));

  ftp_restart(s, atoi(param));
}

//переименовать файл(директорию) из
void do_RNFR(ftp_session_t *s, char *param)
{

  get_system_dir(s, param, s->rename);

  if (is_file_exists(s->rename)) ftp_cmd_out(s, ftp_code_350_file_exists);
  else ftp_cmd_out(s, ftp_code_450_file_unavailable);
}

//переименовать файл(директорию) в
void do_RNTO(ftp_session_t *s,char *param)
{
  char buf[512];

  get_system_dir(s, param, buf);

  if (MoveFile(s->rename, buf)) ftp_cmd_out(s, ftp_code_250_file_complete);
  else ftp_cmd_out(s, ftp_code_450_file_unavailable);
}


//удалить директорию
void do_RMD(ftp_session_t *s, char *param)
{
  char buf[512];

  get_system_dir(s, param, buf);

  if (RemoveDirectory(buf)) ftp_cmd_out(s, ftp_code_250_file_complete);
  else ftp_cmd_out(s, ftp_code_450_file_unavailable);
}

//удалить файл
void do_DELE(ftp_session_t *s, char *param)
{
  char buf[512];

  get_system_dir(s, param, buf);

  if (DeleteFile(buf)) ftp_cmd_out(s, ftp_code_250_file_complete);
  else ftp_cmd_out(s, ftp_code_450_file_unavailable);
}

//создать директорию
void do_MKD(ftp_session_t *s, char *param)
{
  char buf[512];

  get_system_dir(s, param, buf);

  if (CreateDirectory(buf, NULL)) ftp_cmd_out(s, ftp_code_257_dir_create, param);
  else ftp_cmd_out(s, ftp_code_550_no_file);
}

//список файлов в директории
void do_LIST(ftp_session_t *s, char *param)
{
  if (!ftp_connect(s)) {
    ftp_cmd_out(s, ftp_code_425_not_data_connect);
    return;
  }

  ftp_cmd_out(s, ftp_code_150_file_ok);

  WIN32_FIND_DATA find_data;
  HANDLE handle;
  SYSTEMTIME stime;
  char buf[512];
  char year[6], dir, ro, exec;
  SYSTEMTIME time;
  int write, len;


  get_system_dir(s, NULL, buf);
  if (buf[strlen(buf) - 1] == '/') strcat(buf, "*");
  else strcat(buf, "/*");

  GetLocalTime(&stime);


  handle = FindFirstFile(buf, &find_data);
  if (handle == INVALID_HANDLE_VALUE) {
    FindClose(handle);
    ftp_cmd_out(s, ftp_code_450_file_unavailable);
    return;
  }


  do {

    if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)  continue;

    dir = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 'd' : '-';
    ro = (find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)   ? '-' : 'w';
    exec = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 'x' : '-';

    FileTimeToSystemTime(&find_data.ftLastWriteTime, &time);
    if (time.wYear == stime.wYear) sprintf(year, "%02d:%02d", time.wHour, time.wMinute);
    else sprintf(year, "%d", time.wYear);

    if (s->curr_command == FTP_CMD_LIST) {
      sprintf(buf, "%cr%c%cr%c%cr%c%c    1 %-8s %-8s %10lu %s %2d %5s %s\r\n",
                   dir, ro, exec, ro, exec, ro, exec, ftp_owner, ftp_group,
                   find_data.nFileSizeLow, ftp_month[time.wMonth-1],
                   time.wDay, year, find_data.cFileName);
    }
    else  sprintf(buf, "%s\r\n", find_data.cFileName);

    len = strlen(buf);
    write = ftp_data_out(s, buf, len);
    if (write != len) {
      FindClose(handle);
      ftp_disconnect(s);
      ftp_cmd_out(s, ftp_code_451_abort_local);
      return;
    }
  } while (FindNextFile(handle, &find_data));

  FindClose(handle);
  ftp_disconnect(s);
  ftp_cmd_out(s, ftp_code_226_trans_complete);
}


//название ОС
void do_SYST(ftp_session_t *s, char *param)
{
  ftp_cmd_out(s, ftp_code_215_sys_type);
}

//помощь
void do_HELP(ftp_session_t *s, char *param)
{
  char buf[512], *p;
  int i;

  p = buf;
  strcpy(p, "214-Recognized commands. ( * = not implemented )\r\n");
  p += strlen(p);

  for (i = 0; i < MAX_FTP_COMMANDS; i++) {
    p += sprintf(p, (i+1)%6 == 0 ? "  %c%s\r\n" : "  %c%-5s", (ftp_handlers[i] == do_NIMP) ? '*' : ' ', ftp_commands[i]);
  }

  if (i%6 != 0)
      p += sprintf(p, "\r\n");

  ftp_cmd_out(s, ftp_code_214_help, buf);
}

//нет операций
void do_NOOP(ftp_session_t *s, char *param)
{
  ftp_cmd_out(s, ftp_code_200_ok);
}

