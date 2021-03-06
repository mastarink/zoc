#ifndef MAS_DUF_FORMAT_TYPES_H
#  define MAS_DUF_FORMAT_TYPES_H

typedef struct
{
  unsigned dataid:1;
  unsigned dirid:1;
  unsigned dirid_space:1;
  unsigned nfiles:1;
  unsigned nfiles_space:1;
  unsigned ndirs:1;
  unsigned ndirs_space:1;
  unsigned filename:1;
  unsigned filesize:1;
  unsigned human:1;
  unsigned inode:1;
  unsigned sd5:1;
  unsigned sd5id:1;
  unsigned md5:1;
  unsigned md5id:1;
  unsigned crc32:1;
  unsigned crc32id:1;
  unsigned nameid:1;
  unsigned nameid_space:1;
  unsigned mime:1;
  unsigned mimeid:1;
  unsigned mimeid_space:1;
  unsigned exifid:1;
  unsigned exifid_space:1;
  unsigned exifdt:1;
  unsigned camera:1;
  unsigned mode:1;
  unsigned mtime:1;
  unsigned nlink:1;
  unsigned prefix:1;
  unsigned suffix:1;
  unsigned realpath:1;
  unsigned depth:1;
  unsigned seq:1;
  unsigned seq_node:1;
  unsigned seq_leaf:1;
  unsigned group:1;
  unsigned user:1;
  unsigned short_filename:1;
} duf_format_flags_t;

typedef struct
{
  union
  {
    duf_format_flags_t flag;
    unsigned long long bit;
  } v;
  unsigned nsame;
  unsigned offset;
} duf_bformat_combo_t;



typedef enum
{
  DUF_FORMAT_DATAID,
  DUF_FORMAT_DIRID,
  DUF_FORMAT_DIRID_SPACE,
  DUF_FORMAT_NFILES,
  DUF_FORMAT_NFILES_SPACE,
  DUF_FORMAT_NDIRS,
  DUF_FORMAT_NDIRS_SPACE,
  DUF_FORMAT_FILENAME,
  DUF_FORMAT_FILESIZE,
  DUF_FORMAT_HUMAN,
  DUF_FORMAT_INODE,
  DUF_FORMAT_SD5,
  DUF_FORMAT_SD5ID,
  DUF_FORMAT_MD5,
  DUF_FORMAT_MD5ID,
  DUF_FORMAT_CRC32,
  DUF_FORMAT_CRC32ID,
  DUF_FORMAT_NAMEID,
  DUF_FORMAT_MIME,
  DUF_FORMAT_MIMEID,
  DUF_FORMAT_EXIFID,
  DUF_FORMAT_EXIFDT,
  DUF_FORMAT_MODE,
  DUF_FORMAT_MTIME,
  DUF_FORMAT_NLINK,
  DUF_FORMAT_NSAME,
  DUF_FORMAT_OFFSET,
  DUF_FORMAT_PREFIX,
  DUF_FORMAT_SUFFIX,
  DUF_FORMAT_REALPATH,
  DUF_FORMAT_DEPTH,
  DUF_FORMAT_SEQ,
  DUF_FORMAT_SEQ_NODE,
  DUF_FORMAT_SEQ_LEAF,
  /* DUF_FORMAT_TRUEPATH, */
  DUF_FORMAT_USER,
  DUF_FORMAT_GROUP,
  DUF_FORMAT_MAX,
  DUF_FORMAT_BOO,
} duf_format_index_t;



#endif

/*
vi: ft=c
*/
