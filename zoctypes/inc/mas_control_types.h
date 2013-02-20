#ifndef MAS_CONTROL_TYPES_H
#  define MAS_CONTROL_TYPES_H

#  include <stdio.h>

#  include <mastar/types/mas_common_types.h>
/* #  include <mastar/types/mas_log_types.h> */
#  include <mastar/types/mas_variables_types.h>

#  include <mastar/types/mas_listener_control_types.h>
#  include <mastar/types/mas_transaction_control_types.h>

#  define MAS_MAX_PIDFD 5
/*
this:
  mas_control_types.h
more:
  mas_common_types.h
*/
typedef struct mas_pthread_globals_s mas_pthread_globals_t;
struct mas_pthread_globals_s
{
  pthread_mutex_t malloc_mutex;
  /* pthread_mutex_t lcontrols_list_mutex; */
  pthread_rwlock_t lcontrols_list_rwlock;
  pthread_rwlock_t modules_list_rwlock;
  pthread_mutex_t cleanup_transactions_mutex;
  pthread_mutex_t msg_mutex;
  pthread_mutex_t emsg_mutex;
  pthread_mutex_t mfp_mutex;
  pthread_mutex_t cnttr1_mutex;
  pthread_mutex_t cnttr2_mutex;
  pthread_mutex_t cnttr3_mutex;
  pthread_mutex_t logger_mutex;

  pthread_attr_t custom_attr;
  pthread_attr_t master_attr;
  pthread_attr_t ticker_attr;
  pthread_attr_t watcher_attr;
  pthread_attr_t logger_attr;
  pthread_attr_t listener_attr;
  pthread_attr_t transaction_attr;
};


struct mas_control_s
{
  pid_t main_pid;
  pid_t main_tid;
  pthread_t main_thread;

  pid_t child_pid;
  pid_t child_tid;
  pthread_t child_thread;

  pid_t master_pid;
  pid_t master_tid;
  pthread_t master_thread;

  pid_t server_pid;
  pid_t server_tid;
  pthread_t server_thread;

  pthread_t ticker_thread;
  pid_t ticker_tid;

  pthread_t watcher_thread;
  pid_t watcher_tid;

  pthread_t logger_thread;
  pid_t logger_tid;

  char *binname;
  char *progname;
  char *exepath;

  void **loaded_modules;
  unsigned loaded_modules_cnt;

  double start_time;
  double transactions_time;

  int old_stderr;
  FILE *old_stderrfile;
  FILE *stderrfile;
  int old_stdout;
  FILE *msgfile;

  unsigned int_cnt;
  unsigned term_cnt;
  unsigned try_cnt;
  unsigned restart_cnt;
  unsigned fatal:1;
  unsigned main_exit:1;
  unsigned in_client:1;
  unsigned in_pipe;
  unsigned is_client:1;
  unsigned is_server:1;
  unsigned is_parent:1;

  /* from opts: */
  unsigned messages:1;

  unsigned daemon:1;
  unsigned close_std:1;
  unsigned redirect_std:1;
  /* /from opts: */

  unsigned keep_listening:1;
  unsigned keep_logging:1;
  unsigned stop_listeners:1;

  unsigned disconnect:1;
  unsigned restart:1;
  unsigned quit:1;
  unsigned do_quit:1;

  unsigned forget_transactions:1;
  unsigned opts_saved:1;
  unsigned opts_saved_plus:1;
  unsigned ticker_hide:1;
  unsigned watcher_hide:1;
  unsigned watcher_stop:1;

  unsigned log_disabled:1;
  unsigned log_offmem:1;

  mas_pthread_globals_t thglob;

  unsigned long listener_serial;
  /* unsigned npthclients; */
  /* unsigned listening_max; */
  mas_lcontrol_list_head_t *lcontrols_list;
  /* mas_loginfo_list_head_t *log_list; */
  mas_variables_list_head_t *hostvars;
  char *const *launcherv;
  int launcherc;
  char *const *launchere;
  int argv_nonoptind;

  size_t protos_num;
  mas_transaction_protodesc_t *proto_descs;

  mas_stamp_t stamp;

  mas_status_t status;
  unsigned long log_q_came;
  unsigned long log_q_gone;
  unsigned long log_q_mem;
  unsigned long clients_came0;
  unsigned long clients_came;
  unsigned long clients_gone;
  unsigned long clients_gone2;
  unsigned long xch_cnt;
  unsigned long maxclients;
  double maxtrtime;
  unsigned long sigint_time;
  FILE *pidfile;
  int pidfd[MAS_MAX_PIDFD];
  char *logpath;
  FILE *logfile;
  int commands_num;
  char **commands;

  pthread_key_t mas_thread_key;
  pthread_once_t mas_thread_key_once;
};
typedef struct mas_control_s mas_control_t;

#endif
