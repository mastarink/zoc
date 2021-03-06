#ifndef MASXFS_LEVINFO_IO_DIR_H
# define MASXFS_LEVINFO_IO_DIR_H

# include "masxfs_levinfo_types.h"

int masxfs_levinfo_opendir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, masxfs_entry_filter_t * entry_pfilter );
int masxfs_levinfo_closedir_force( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int rcode );
int masxfs_levinfo_opened_dir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags, int def );
int masxfs_levinfo_closedir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

int masxfs_levinfo_closedir_all_up( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );
int masxfs_levinfo_readdir( masxfs_levinfo_t * li, masxfs_entry_filter_t * entry_pfilter, masxfs_levinfo_flags_t flags, int *phas_data );
int masxfs_levinfo_rewinddir( masxfs_levinfo_t * li, masxfs_levinfo_flags_t flags );

#endif
