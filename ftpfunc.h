#ifndef ftpfuncH
#define ftpfuncH

#include "ftpserv.h"
#include "ftpconst.h"

void do_ACCEPT(ftp_session_t * s, char * param);
void do_USER(ftp_session_t *s, char *param);
void do_PASS(ftp_session_t *s, char *param);
void do_ACCT(ftp_session_t *s, char *param);
void do_CWD(ftp_session_t *s, char *param);
void do_CDUP(ftp_session_t *s, char *param);
void do_SMNT(ftp_session_t *s, char *param);
void do_REIN(ftp_session_t *s, char *param);
void do_QUIT(ftp_session_t *s, char *param);
void do_PORT(ftp_session_t *s, char *param);
void do_PASV(ftp_session_t *s, char *param);
void do_TYPE(ftp_session_t *s, char *param);
void do_STRU(ftp_session_t *s,char *param);
void do_MODE(ftp_session_t *s, char *param);
void do_RETR(ftp_session_t *s, char *param);
void do_STOR(ftp_session_t *s, char *param);
void do_STOU(ftp_session_t *s, char *param);
void do_APPE(ftp_session_t *s, char *param);
void do_ALLO(ftp_session_t *s, char *param);
void do_REST(ftp_session_t *s, char *param);
void do_RNFR(ftp_session_t *s, char *param);
void do_RNTO(ftp_session_t *s,char *param);
void do_ABOR(ftp_session_t *s,char *param);
void do_DELE(ftp_session_t *s, char *param);
void do_RMD(ftp_session_t *s, char *param);
void do_MKD(ftp_session_t *s, char *param);
void do_PWD(ftp_session_t *s, char *param);
void do_LIST(ftp_session_t *s, char *param);
void do_NLST(ftp_session_t *s, char *param);
void do_SITE(ftp_session_t *s, char *param);
void do_SYST(ftp_session_t *s, char *param);
void do_STAT(ftp_session_t *s, char *param);
void do_HELP(ftp_session_t *s, char *param);
void do_NOOP(ftp_session_t *s, char *param);
void do_XCWD(ftp_session_t *s, char *param);
void do_XCUP(ftp_session_t *s, char *param);
void do_XRMD(ftp_session_t *s, char *param);
void do_XMKD(ftp_session_t *s, char *param);
void do_XPWD(ftp_session_t *s, char *param);
void do_NIMP(ftp_session_t *s, char *param); // NIMP = not implemented


extern const FTP_HANDLER ftp_handlers[MAX_FTP_COMMANDS];

#endif
