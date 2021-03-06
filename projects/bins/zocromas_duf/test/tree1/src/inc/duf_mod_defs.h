#ifndef DUF_MOD_DEFS_H
#define DUF_MOD_DEFS_H

#define DUF_MOD_DECLARE_ALL_FUNCS( _modname ) \
static int _modname ## _init(              duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_before2(      duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_before2_del(  duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_after2(       duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_after2_del(   duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_middle2(      duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _node_middle2_del(  duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _de_content2(       duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _de_content2_del(   duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _leaf2(             duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _leaf2_del(         duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _de_file_before2(   duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi ); \
static int _modname ## _de_dir_before2(    duf_sqlite_stmt_t * pstmt_unused, duf_depthinfo_t * pdi );

#endif
