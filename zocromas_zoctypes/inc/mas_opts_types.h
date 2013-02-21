#ifndef MAS_OPTS_TYPES_H
#  define MAS_OPTS_TYPES_H

struct mas_options_s
{
  union mas_options_u
  {
    unsigned long word;
    struct mas_optflags_s
    {
      unsigned msg_c:1;
      unsigned msg_s:1;
      /* main */
      unsigned msg_m:1;
      /* communication */
      unsigned msg_io:1;
      /* listener */
      unsigned msg_l:1;
      /* transaction */
      unsigned msg_r:1;
      /* waiter */
      unsigned msg_w:1;
      unsigned msg_th:1;
      /* signal */
      unsigned msg_sg:1;
      /* trace */
      unsigned msg_tr:1;
      unsigned msg_trace_main:1;
      unsigned msg_trace_listener:1;
      unsigned msg_trace_transaction:1;
      unsigned msg_cmd:1;
      unsigned msg_mem:1;
      unsigned msg_funline:1;
    } bit;
  } f;
  unsigned single_instance:1;
  unsigned single_child:1;
  unsigned listener_single:1;
  unsigned transaction_single:1;
  unsigned nomessages:1;
  
  unsigned nodaemon:1;
  unsigned noclose_std:1;
  unsigned noredirect_std:1;

  unsigned nologger:1;
  unsigned nolog:1;
  unsigned nowatcher:1;
  unsigned save_opts:1;
  unsigned save_opts_plus:1;
  unsigned disconnect_prompt:1;
  unsigned wait_server:1;
  unsigned make_master_thread:1;
  unsigned noticker:1;
  unsigned ticker_mode;
  unsigned nomaster;
  unsigned nolistener;
  unsigned nolisten;
  unsigned exitsleep;
  unsigned default_port;
  char env_optsname[256];
  char env_hostname[256];
  unsigned argc;
  char **argv;
  int protos_num;
  char **protos;
  int hosts_num;
  char **hosts;
  int commands_num;
  char **commands;
  unsigned client_attempts;
  char *uuid;
  char *msgfilename;
  char *pidsdir;
  char *modsdir;
  char *protodir;
  char *logdir;
  char *configdir;
  char *configfilename;
  char *saved_configfilename;
  unsigned max_config_backup;
  double restart_sleep;
};
typedef struct mas_options_s mas_options_t;



#endif