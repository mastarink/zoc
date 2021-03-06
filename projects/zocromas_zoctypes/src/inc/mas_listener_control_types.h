#ifndef MAS_LISTENER_CONTROL_TYPES_H
#  define MAS_LISTENER_CONTROL_TYPES_H

#  include <mastar/types/mas_varset_types.h>
#  include <mastar/types/mas_autoset_types.h>
#  include <mastar/types/mas_transaction_control_types.h>

/*
this:
  mas_listener_control_types.h
related:
  mas_common_types.h
  mas_transaction_control_types.h
*/

// *INDENT-OFF*
typedef MAS_LIST_HEAD( mas_lcontrol_list_s, mas_lcontrol_s ) mas_lcontrol_list_head_t;
typedef MAS_LIST_ENTRY( mas_lcontrol_s ) mas_lcontrol_list_entry_t;
// *INDENT-ON*

typedef struct mas_lcontrol_s
{
  mas_common_control_t c;
  struct mas_ocontrol_s h;

  mas_lcontrol_list_entry_t next;

  mas_rcontrol_list_head_t *transaction_controls_list;

  const struct mas_options_s *popts;

  char *host;
  size_t hostlen;
  unsigned port;
  /* unsigned transactions_max; */
  /* pthread_mutex_t transaction_mutex; */
  mas_autoset_t *autoset;
  pthread_mutex_t autoset_mutex;
  pthread_rwlock_t transaction_rwlock;
  pthread_rwlock_t variables_rwlock;
  unsigned long transaction_serial;
  unsigned long clients_came;
  unsigned long clients_gone;
  mas_varset_t *variables;
  unsigned in_list:1;
} mas_lcontrol_t;

/* struct mas_lcontrol_s                                  */
/* {                                                      */
/*   unsigned id;                                         */
/*   const char *host;                                    */
/*   unsigned port;                                       */
/*   pthread_t thread;                                    */
/*   unsigned transactions_max;                           */
/*   mas_r_control_t *transaction_controls_array;         */
/*   mas_rcontrol_list_head_t *transaction_controls_list; */
/*   struct mas_channel_s *pchannel;                      */
/*   mas_status_t status;                                 */
/*   mas_lcontrol_list_entry_t next;                      */
/* };                                                     */
/* typedef struct mas_lcontrol_s mas_lcontrol_t;          */



#endif
