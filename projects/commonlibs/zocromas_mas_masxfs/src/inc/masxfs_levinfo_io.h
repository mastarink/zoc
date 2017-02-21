#ifndef MASXFS_LEVINFO_IO_H
# define MASXFS_LEVINFO_IO_H

# include "masxfs_types.h"
int masxfs_levinfo_is_open( masxfs_levinfo_t * li );
int masxfs_levinfo_fd( masxfs_levinfo_t * li );
int masxfs_levinfo_open( masxfs_levinfo_t * li );
int masxfs_levinfo_opendir( masxfs_levinfo_t * li );
int masxfs_levinfo_close( masxfs_levinfo_t * li );
int masxfs_levinfo_close_all_up( masxfs_levinfo_t * li );

masxfs_dir_t *masxfs_levinfo_opendir_up( masxfs_levinfo_t * li );
int masxfs_levinfo_closedir( masxfs_levinfo_t * li );
int masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li );

masxfs_dirent_t *masxfs_levinfo_readdir( masxfs_levinfo_t * li );
int masxfs_levinfo_rewinddir( masxfs_levinfo_t * li );
int masxfs_levinfo_have_more( masxfs_levinfo_t * li, masxfs_entry_type_t detype );

int masxfs_levinfo_stat( masxfs_levinfo_t * li );

#endif
