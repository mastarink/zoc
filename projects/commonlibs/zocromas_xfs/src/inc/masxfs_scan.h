#ifndef MASXFS_SCAN_H
# define MASXFS_SCAN_H

# include "masxfs_types.h"

int masxfs_scanpath_real( const char *path, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * callbacks, void *userdata, void *userdata2,
                          masxfs_levinfo_flags_t flags );
int masxfs_scanpath_real2( const char *cpath, masxfs_type_flags_t typeflags, masxfs_entry_callback_t * callbacks, void *userdata, void *userdata2,
                           masxfs_levinfo_flags_t flags );

#endif
