//---------------------------------------------------------------------------

#pragma hdrstop

#include "ftpserv.h"
#include <string.h>
#include <stdio.h>
#include "ftpfunc.h"


bool ftp_match_crlf(char * param)
{
  char * ptr;

  ptr = strstr(param, "\r\n");
  if (!ptr) {
    ptr = strstr(param, "\n");
    if (!ptr) return false;
  }

  *ptr = '\0';
  return true;
}

int ftp_parse_command(char * data)
{
  int i;

  for (i = 0; i < MAX_FTP_COMMANDS; ++i) {
    if (memcmp(data, ftp_commands[i], strlen(ftp_commands[i])) == 0) {
      if ( (data[strlen(ftp_commands[i])] == ' ') || (data[strlen(ftp_commands[i])] == '\0') ) return i;
    }
  }
  return -1;
}

void ftp_cmd_in(ftp_session_t * s, char * data)
{
  int rc;

  if (!ftp_match_crlf(data)) {
    ftp_cmd_out(s, ftp_code_501_syntax_err_param);
    return;
  }


  s->curr_command = ftp_parse_command(data);
  if (s->curr_command < 0) {
    ftp_cmd_out(s, ftp_code_502_cmd_not_implement);
    return;
  }

  ftp_handlers[s->curr_command](s, data + strlen(ftp_commands[s->curr_command]) + 1);
}






