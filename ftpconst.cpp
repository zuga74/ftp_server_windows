#include "ftpconst.h"
#include "ftpconf.h"
#include <string.h>


const char * ftp_commands[MAX_FTP_COMMANDS] = {
    // Access Control Commands
    "USER", "PASS", "ACCT", "CWD",
    "CDUP", "SMNT", "REIN", "QUIT",
    // Transfer Parameter Commands
    "PORT", "PASV", "TYPE", "STRU",
    "MODE",
    // FTP Service Commands
    "RETR", "STOR", "STOU", "APPE",
    "ALLO", "REST", "RNFR", "RNTO",
    "ABOR", "DELE", "RMD",  "MKD",
    "PWD",  "LIST", "NLST", "SITE",
    "SYST", "STAT", "HELP", "NOOP",
    // From RFC 1123, we just translate it to standard command
    "XCWD", "XCUP", "XRMD", "XMKD",
    "XPWD"
};




const char ftp_code_200_ok[] = "200 Command okay";
const char ftp_code_500_cmd_unrec[] = "500 Syntax error, command unrecognized.";
const char ftp_code_501_syntax_err_param[] = "501 Syntax error in parameters or arguments.";
const char ftp_code_202_cmd_not_implement_super[] = "202 Command not implemented superflous.";
const char ftp_code_502_cmd_not_implement[] = "502 Command not implemented.";
const char ftp_code_503_bad_seq[] = "503 Invalid sequence of commands (AUTH TLS required prior to authentication).";
const char ftp_code_504_cmd_not_implement_param[] = "504 Command not implemented for that parameter.";

const char ftp_code_110_restart_mark[] = "110 MARK yyyy = mmmm.";
const char ftp_code_211_sys_status[] = "211 System status, or system help reply.";
const char ftp_code_212_dir_status[] = "212 Directory status.";
const char ftp_code_213_file_status[] = "213 File status.";
const char ftp_code_214_help[] = "214 Help message %s.";
const char ftp_code_215_sys_type[] = "215 "FTP_SYSTEM_TYPE;


const char ftp_code_120_serv_ready_min[] = "120 Service ready in %03d minutes.";
const char ftp_code_220_serv_ready[] = "220 "FTP_SERVER_NAME" server ready";
const char ftp_code_221_bye[] = "221 Goodbye!";
const char ftp_code_421_serv_not_available[] = "421 Service not available, closing control connection.";
const char ftp_code_125_trans_start[] = "125 Data connection already open; Transfer starting.";
const char ftp_code_225_trans_bad[] = "225 Data connection already open; Transfer not starting.";
const char ftp_code_425_not_data_connect[] = "425 Can not open data connection.";
const char ftp_code_226_trans_complete[] = "226 Transfer complete. Closing data connection.";
const char ftp_code_426_trans_abort[] = "426 Connection closed; transfer aborted.";
const char ftp_code_227_pas_mode[] = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).";


const char ftp_code_230_user_log[] = "230 User logged in, proceed.";
const char ftp_code_530_user_not_log[] = "530 Please login with USER and PASS.";
const char ftp_code_331_user_ok[] = "331 User name okay, need password.";
const char ftp_code_332_need_acc[] = "332 Need account for login.";
const char ftp_code_532_need_acc_stor[] = "532 Need account for storing files.";
const char ftp_code_150_file_ok[] = "150 File status okay; about to open data connection.";
const char ftp_code_250_file_complete[] = "250 Requested file action okay, completed.";

const char ftp_code_257_dir_create[] = "257 \"%s\" created.";
const char ftp_code_257_dir[] = "257 \"%s\" is current directory.";
const char ftp_code_257_enter_pasv_mode[] = "257 Entering Passive Mode (%d,%d,%d,%d,%d,%d).";



const char ftp_code_350_file_exists[] = "350 File exists. Ready to rename.";
const char ftp_code_350_restarting[] = "350 Restarting at %d.";

const char ftp_code_450_file_unavailable[] = "450 File or directory unavailable.";
const char ftp_code_550_no_file[] = "550 No such file or directory.";
const char ftp_code_451_abort_local[] = "451 Requested action aborted. Local error in processing.";
const char ftp_code_551_abort_unknow[] = "551 Requested action aborted: page type unknown.";
const char ftp_code_452_no_space[] = "452 Insufficient storage space.";
const char ftp_code_552_abort_file[] = "552 Requested file action aborted.";
const char ftp_code_553_file_not_allowed[] = "553 File name not allowed.";



const char ftp_owner[9] = "owner";
const char ftp_group[9] = "group";

const char *ftp_month[] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

