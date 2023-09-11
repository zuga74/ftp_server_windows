#ifndef ftpconstH
#define ftpconstH

#define MAX_FTP_COMMANDS        38

enum {
    // Access Control Commands
    FTP_CMD_USER = 0, FTP_CMD_PASS, FTP_CMD_ACCT, FTP_CMD_CWD,
    FTP_CMD_CDUP, FTP_CMD_SMNT, FTP_CMD_REIN, FTP_CMD_QUIT,
    // Transfer Parameter Commands
    FTP_CMD_PORT, FTP_CMD_PASV, FTP_CMD_TYPE, FTP_CMD_STRU,
    FTP_CMD_MODE,
    // FTP Service Commands
    FTP_CMD_RETR, FTP_CMD_STOR, FTP_CMD_STOU, FTP_CMD_APPE,
    FTP_CMD_ALLO, FTP_CMD_REST, FTP_CMD_RNFR, FTP_CMD_RNTO,
    FTP_CMD_ABOR, FTP_CMD_DELE, FTP_CMD_RMD,  FTP_CMD_MKD,
    FTP_CMD_PWD,  FTP_CMD_LIST, FTP_CMD_NLST, FTP_CMD_SITE,
    FTP_CMD_SYST, FTP_CMD_STAT, FTP_CMD_HELP, FTP_CMD_NOOP,
    // From RFC 1123, we just translate it to standard command
    FTP_CMD_XCWD, FTP_CMD_XCUP, FTP_CMD_XRMD, FTP_CMD_XMKD,
    FTP_CMD_XPWD
};

extern const char * ftp_commands[MAX_FTP_COMMANDS];



extern const char ftp_code_200_ok[];
extern const char ftp_code_500_cmd_unrec[];
extern const char ftp_code_501_syntax_err_param[];
extern const char ftp_code_202_cmd_not_implement_super[];
extern const char ftp_code_502_cmd_not_implement[];
extern const char ftp_code_503_bad_seq[];
extern const char ftp_code_504_cmd_not_implement_param[];

extern const char ftp_code_110_restart_mark[];
extern const char ftp_code_211_sys_status[];
extern const char ftp_code_212_dir_status[];
extern const char ftp_code_213_file_status[];
extern const char ftp_code_214_help[];
extern const char ftp_code_215_sys_type[];


extern const char ftp_code_120_serv_ready_min[];
extern const char ftp_code_220_serv_ready[];
extern const char ftp_code_221_bye[];
extern const char ftp_code_421_serv_not_available[];
extern const char ftp_code_125_trans_start[];
extern const char ftp_code_225_trans_bad[];
extern const char ftp_code_425_not_data_connect[];
extern const char ftp_code_226_trans_complete[];
extern const char ftp_code_426_trans_abort[];
extern const char ftp_code_227_pas_mode[];


extern const char ftp_code_230_user_log[];
extern const char ftp_code_530_user_not_log[];
extern const char ftp_code_331_user_ok[];
extern const char ftp_code_332_need_acc[];
extern const char ftp_code_532_need_acc_stor[];
extern const char ftp_code_150_file_ok[];
extern const char ftp_code_250_file_complete[];

extern const char ftp_code_257_dir[];
extern const char ftp_code_257_dir_create[];
extern const char ftp_code_257_enter_pasv_mode[];

extern const char ftp_code_350_file_exists[];
extern const char ftp_code_350_restarting[];
extern const char ftp_code_450_file_unavailable[];
extern const char ftp_code_550_no_file[];
extern const char ftp_code_451_abort_local[];
extern const char ftp_code_551_abort_unknow[];
extern const char ftp_code_452_no_space[];
extern const char ftp_code_552_abort_file[];
extern const char ftp_code_553_file_not_allowed[];



extern const char ftp_owner[9];
extern const char ftp_group[9];

extern const char *ftp_month[];


#define IS_TYPE_CODE(C) (C == 'A' || C == 'E' || C == 'I' || C == 'L')
#define IS_MODE_CODE(C) (C == 'S' || C == 'B' || C == 'C')
#define IS_STRU_CODE(C) (C == 'F' || C == 'R' || C == 'P')
#define IS_FORM_CODE(C) (C == 'N' || C == 'T' || C == 'C')



#endif
