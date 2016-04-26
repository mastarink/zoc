# set logging off
# set logging redirect on
# set logging file /tmp/gdb.log
set print thread-events off
set print elements 0
define hook-quit
    set confirm off
end
handle SIGPIPE nostop noprint
set breakpoint pending on

#b muc_option_longopts.c:92
b muc_option_config.c:321
#b duf_sccb_row.c:200
# b duf.c:232
# b duf_sccbh_eval_sql_set.c:270
# b duf_sccbh_eval_leaf.c:119

# watch lo_extended_vtable_multi

# watch global_status.pdilist
# watch global_status.pdilist->next


# commands 1
# print duf_config->scn.pdi->pathinfo.levinfo[duf_config->scn.pdi->pathinfo.depth].itemname[0]
# end


# display &(duf_config->scn.pdi->pathinfo.levinfo[duf_config->scn.pdi->pathinfo.depth].itemname)
# display duf_config->scn.pdi->num_idstatements
# display &(duf_config->scn.pdi->idstatements[duf_config->scn.pdi->num_idstatements-1])



run

# display pdi.pathinfo->levinfo[d].itemname
# bt
# vi: ft=gdb
