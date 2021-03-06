#ifndef MAS_QSTD_TYPES_H
# define MAS_QSTD_TYPES_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>

typedef struct stat mas_qstd_stat_t;

# include <mastar/mysqlpfs/mysqlpfs_types.h>

typedef enum mas_qstd_id_e mas_qstd_id_t;
typedef enum mas_qstd_nfields_e mas_qstd_nfields_t;
typedef struct mas_qstd_s mas_qstd_t;

#endif
