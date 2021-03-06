#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_options_file.h>
#include <mastar/multiconfig/muc_option_names.h>
#include <mastar/multiconfig/muc_option_descr.h>
#include <mastar/multiconfig/muc_option_config.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_output_defs.h"
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

/* ###################################################################### */
#include "duf_optimpl_example.h"
/* ###################################################################### */

SR( SNIPPET_OPTION, optimpl_O_examples, /* int argc, char *const *argv */ void )
{
/* DUF_STARTR( r ); */

  DUF_PRINTF( 0, "Examples" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --allow-drop-tables --allow-create-tables" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file" );
  DUF_PRINTF( 0,
              "  run  --db-name=test20140412  --allow-drop-tables --allow-create-tables  --add-path /home/mastar/a/down/  --uni-scan -R --collect --dirent --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/chromium/   --print    -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print  --tree  -R --files" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print   -R --files" );
  DUF_PRINTF( 0, "----------------" );
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-duplicates" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-exif" );
#endif
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -d -R" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample=2 /mnt/new_media/media/down/ --max-seq=10" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -Ri5fd /home/mastar/a/down/ --trace-md5 --trace-new=0 --trace-stdout" );

  DUF_PRINTF( 0, "===== Transition ============================" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  run --update-duplicates --update-mdpath --update-filedata --update-exif" );
#endif
  DUF_PRINTF( 0, "  run --db-name=test`datem`.db  --uni-scan  --mdpath --collect --dirent -dfR  /mnt/new_media/media/down/" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "Collect basic file info recursively to db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db   --uni-scan -R --collect --dirent --files   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "Calculate checksums recursively for files in db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -R --collect --dirent --md5 --files --dirs /home/mastar/a/down/" );
  DUF_PRINTF( 0, "List files recursively from db" );
  DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan   --print -R -d  --files /home/mastar/a/down/google-chrome/ " );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run --db-name=test.db  -AAPB   /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "    same as prev:" );
  DUF_PRINTF( 0, "  run --db-name=test.db --trace-action=2 --allow-create-tables --uni-scan --recursive --collect --dirent"
              " --files --dirs --md5 /mnt/new_media/media/down/ --summary" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan --print  --md5 --noopenat -Rdf --max-dirs=300  --min-dirfiles=5 --min-size=10" );
  DUF_PRINTF( 0, "=============================================" );

  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6 --summary"
              " --format=filesize,seq,filename,md5" );
  DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6"
              " --summary --format=filesize,seq,filename,md5 --tree" );
  DUF_PRINTF( 0, "  run  --trace-any=0  --db-name=test20140416.db  --uni-scan   --print  -df --max-seq=76 --max-depth=6 --summary"
              " --format=filesize,seq,filename,nlink,mtime,mode,gid,uid /mnt/new_media/media/down" );
  DUF_PRINTF( 0, "=============================================" );
  DUF_PRINTF( 0, "  run  --db-name=test$$.db  --uni-scan --print  --md5 --noopenat  -Rdf --max-dirs=300 --max-items=200 "
              " --min-dirfiles=5 --min-size=10 --noopenat" );

  DUF_PUTS( 0, "" );
  DUF_PUTS( 0, NULL );
  DUF_PUTSL( 0 );
  DUF_PRINTF( 0, "=== Working with the tree as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --allow-remove-database" );
  DUF_PRINTF( 0, "  run -OPA /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run --uni-scan -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run -RdifE5 /home/mastar/a/down/chromium/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf" );
  DUF_PRINTF( 0, "========================= as for 20140429 ===================" );
  DUF_PRINTF( 0, "  run --sample=2 -Rdf /home/mastar/a/down/////  --printf --max-seq=2000" );
  DUF_PRINTF( 0, "========================= as for 20140501 ===================" );
  DUF_PRINTF( 0, "  run -PRdifE5 /home/mastar/a/down/chromium/ -SSS" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rd --summary  --tree   /home/mastar/a/down/" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf --max-seq=1000" );
  DUF_PRINTF( 0, "========================= as for 20140504 ===================" );
  DUF_PRINTF( 0, "= full - cycle collect in one command =======================" );
  DUF_PRINTF( 0, "  run  --allow-remove-database -OP  -RdifE5 /home/mastar/a/down/chromium/"
              " --exclude='*.doc' --exclude='*.jpg' --exclude='*.css'" " --exclude='*.html' --exclude='*.png'  --exclude='*.gif'"
              " --exclude='*.js'  --exclude='*.php'" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-10 --size=+0" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-1b" );
  DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=+1G" );
  DUF_PRINTF( 0, "=============================================================" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --same=+1 --format=filename,md5id" );
  DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --md5id=1285" );
  DUF_PRINTF( 0, "========================= as for 20140505 ===================" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/ --md5id=121549" " --format=human,inode=0,dataid=0" );
  DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/" " --format=human,inode=0,dataid=0,seq=0,prefix=0" " --include='*Speckled*'" );
  DUF_PRINTF( 0, "========================= as for 20140506 ===================" );
  DUF_PRINTF( 0, "  run -O -Rdif --mime /mnt/new_media/media/photo --max-seq=2000 (%gsec)", 2.8 );
  DUF_PRINTF( 0, "========================= as for 20140509 ===================" );
  DUF_PRINTF( 0, "  run --allow-remove-database" );
  DUF_PRINTF( 0, "  run   -O						- %s", "creates DB" );
  DUF_PRINTF( 0, "  run   -P     /mnt/new_media/media/photo		- %s", "adds initial path" );
  DUF_PRINTF( 0, "  run   -OP    /mnt/new_media/media/photo		- %s", "creates DB AND adds initial path" );
  DUF_PRINTF( 0, "  run   -RdE   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rd    /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rdf   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -Rid   /mnt/new_media/media/photo		- %s", "no-action status" );
  DUF_PRINTF( 0, "  run   -RidE  /mnt/new_media/media/photo		- %s", "collect directories information" );
  DUF_PRINTF( 0, "  run   -RiDE  /mnt/new_media/media/photo		- %s", "        files data (nameless) information" );
  DUF_PRINTF( 0, "  run   -RinE  /mnt/new_media/media/photo		- %s", "        files names information" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo		- %s", "        files md5 information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -Rife  /mnt/new_media/media/photo		- %s", "        files mime information (-f pre-open file)" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo		- %s", "print tree w/o files" );
  DUF_PRINTF( 0, "  run   -pRTdf /mnt/new_media/media/photo		- %s", "      tree with files" );
  DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo --max-depth=2	- %s", "      tree with files limited depth" );
  DUF_PRINTF( 0, "  run   -pRdf  /mnt/new_media/media/photo		- %s", "      files (full paths) with dir headers linear" );
  DUF_PRINTF( 0, "  run   -pRd   /mnt/new_media/media/photo		- %s", "      dirs (full paths) linear" );
  DUF_PRINTF( 0, "  run   -pRf   /mnt/new_media/media/photo		- %s", "      files (full paths) linear" );
  DUF_PRINTF( 0, "========================= as for 20140510 ===================" );
  DUF_PRINTF( 0, "  run   -Rdf --sample  --output /mnt/new_media/media/photo/" );
  DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo --progress" );
  DUF_PRINTF( 0, "========================= as for 20140513 ===================" );
  DUF_PRINTF( 0, "  run   -RifX  /mnt/new_media/media/photo		- %s", "        files exif information (-f pre-open file)" );
  DUF_PRINTF( 0, "========================= as for 20140717 ===================" );
  DUF_PRINTF( 0, "  export MSH_DUF_OPTIONS=explain=1			- %s", "" );
  DUF_PRINTF( 0, "  run   -RidDnE  /mnt/new_media/media/photo		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140718 ===================" );
  DUF_PRINTF( 0, "  run   -OPRidDnf5E  /mnt/new_media/media/photo		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140818 ===================" );
  DUF_PRINTF( 0, "  run   --help-class-short 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class       		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-help   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-system 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-refer		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-collect		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-scan   		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-update 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-request		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-print  		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-nodesc 		- %s", "" );
  DUF_PRINTF( 0, "  run   --help-class-examples		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140828 ===================" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/ -iPO --progress  --md5 --sd5 --crc32 --mime --exif --dirs --files --dirent --filenames --filedata --add-path --allow-create-tables --allow-drop-tables --allow-remove-database --allow-vacuum  --print --integrity --recursive --info  --disable-calculate  --disable-update --summary --version		- %s",
              "--integrity is obsolete: 20151004.132149" );
  DUF_PRINTF( 0, "  run  -OPRdEifndD -532Xe /mnt/new_media/media/photo/ --trace-options		- %s", "" );
  DUF_PRINTF( 0, "  run --print -Rdf --max-depth=2   /mnt/new_media/media/photo/		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140831 ===================" );

  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/dvd/benq/benq/  -Rpdf --min-size=600000 --min-same=6 --md5id=18468		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --inode=61080661		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --nameid=10000		- %s", "" );
  DUF_PRINTF( 0, "  run   /mnt/new_media/media/photo/  -Rpdf --size=181684411		- %s", "" );
  DUF_PRINTF( 0, "  run  -OPRdEifndD -523Xe /mnt/new_media/media/photo/		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140901 ===================" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/  -Rpdf --format=human=0,mtime,nameid=0,dirid=0,inode=0,md5=0 "
              "--min-mtime=\"'1999-01-01 00:00:00'\" --max-mtime=\"'2002-08-2 00:00:00'\" 		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140904 ===================" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same --glob='*06-unknown.jpeg' 		- %s", "" );
  DUF_PRINTF( 0, "  shn g --memusage  /mnt/new_media/media/photo/  -Rpdf --same --glob='*06-unknown.jpeg' 		- %s", "" );
  DUF_PRINTF( 0, "  run -OPREif -X /mnt/new_media/media/photo/ --progress 		- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20140905.111356 ============" );
  DUF_PRINTF( 0,
              "  run  /mnt/new_media/media/photo/   -Rpdf  --output-level=6 "
              " --same-as=/mnt/new_media/media/photo/Pictures/photos/sel/we/we9.jpg 		- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20140907.070802 ============" );
  DUF_PRINTF( 0, "  run -OPRdEi /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s", "collect dirs from filesystem only" );
  DUF_PRINTF( 0, "  run -OPRdEin /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s",
              "collect dirs and filenames from filesystem only; no file info like permissions, size etc." );
  DUF_PRINTF( 0, "  run -OPRdEiD /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s",
              "collect dirs and file data from filesystem only; no file names" );
  DUF_PRINTF( 0, "  run -OPRdEinD /mnt/new_media/media/photo/Pictures/photos/ --progress 		- %s", "collect dirs and files from filesystem only" );
  DUF_PRINTF( 0, "  run -OPRdDnEi -f5 /mnt/new_media/media/photo/Pictures/photos/ --progress	- %s",
              "collect dirs and files from filesystem, count md5" );
  DUF_PRINTF( 0, "  run -OPRdDnEi  -f523Xe --progress /mnt/new_media/media/photo/Pictures/photos/	- %s", "collect everything we can" );

  DUF_PRINTF( 0, "========================= as for 20150124.154800 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -RDdnEiX235fOe   --show-sccbs --nosccbs	- %s", "dbg ..." );
  DUF_PRINTF( 0, "========================= as for 20150128.161052 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/ -P --trace-path=3	- %s", "dbg, see human/screen-20150128.160747.png" );

  DUF_PRINTF( 0, "========================= as for 20150506.153411 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --min-size=300M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --min-size=200M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=200M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=20M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2M -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2K -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=2k -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=1k -T	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf   --max-size=1k --min-size=1 -T > `datemt`.tt	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150509.152057 ============" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/  --allow-remove-database  -PRDdnEiX235fOep --progres    	- %s", "" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/   -Rpdf     --min-same=10 --output-level=4    	- %s", "" );
  DUF_PRINTF( 0, "  run /home/mastar/.mas/lib/mpd/music/   -Rpdf     --same=5 -T    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150517.113041 ============" );
  DUF_PRINTF( 0, "  run -pdf /mnt/new_media/media/audio/music/best/world/  --max-seq=30 --min-same=9    -R    	- %s", "" );
  DUF_PRINTF( 0, "  run --memusa --flags    	- %s", "" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/     --min-exifdt=20120101 --max-exifdt=20130101  -dfR  --evaluate-sccb=listing    	- %s", "" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --list-sccbs    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150520.114602 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO -dEi   	- %s",
              "insert direct child dirs into db; " " without -i will insert only 'upper' (containted in the path)" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO  --evaluate-sccb=dirs    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO -dEiR   	- %s", "insert directly, recursively dirs into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO -R --evaluate-sccb=dirs  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.121030 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO  -dEiD   	- %s",
              "(-D is for filedata) insert direct child dirs,filedata into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO  --evaluate-sccb=dirs --evaluate-sccb=filedata  	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.122922 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO -dEiDn   	- %s",
              "(-n if for filenames) insert direct child dirs,filedata,filenames into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO --evaluate-sccb=dirs,filedata,filenames  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.122922 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO -dEiDn3	- %s",
              "(-3) insert direct child dirs,filedata,filenames,crc32 into db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO --evaluate-sccb=dirs,filedata,filenames,crc32	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.175243 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fi3ER	- %s", "(-3) add crc32 data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=crc32    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.175243 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fi2ER	- %s", "(-2) add sd5 data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=sd5    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150520.220258 +===========" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --trace-mod -f --evaluate-sccb=dummy   	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150521.082012 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fieER	- %s", "(-e) add mime data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=mime   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150521.082052 ============" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fiXER	- %s", "(-X) add exif data to db" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --progress -fR --evaluate-sccb=exif   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/ --allow-remove-database -PO --progress -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif	- %s", "all from scratch..." );
  DUF_PRINTF( 0, "  run /mnt/new_media/media/photo/Pictures/one/ --allow-remove-database -PO --progress -fR "
              " --evaluate-sccb=dirs,filedata,filenames,md5,mime,exif   	- %s", "for faster testing use this path" );

  DUF_PRINTF( 0, "========================= as for 20150522.100438 ============" );
  DUF_PRINTF( 0, "  run  --db-name=temp.db.big20150522.124517 /home/mastar/.maslib/firefox/scrapbook -dfR "
              " --min-same=900 --max-same=3000  --evaluate-sccb=listing    	- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20150526.150510 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -p"
              " --format-dirs-list='-%%_%%f%%_------%%_%%r\\n'"
              " --format-files-list='%%-4S%%5_%%f%%4s%%8z%%4_%%m%%_%%tm%%_/%%_%%T\\n'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -T" " --glob=IMAG1248.jpg "
              " --format-dirs-tree='%%f\\n' " " --format-files-tree='%%f%%4s%%8z%%4_%%m%%_%%tm%%_/%%_%%T\\n' "
              " --format-prefix-files-tree='%%2_#%%-6M%%6_%%P'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -fR -p --glob=IMAG1248.jpg "
              " --format-files-list='%%2_=%%-4S%%5_%%5z%%4_%%m%%_%%tm%%_/%%_%%T%%_%%r%%f\\n'    	- %s", "" );
  DUF_PRINTF( 0, "========================= as for 20150528.122815 ============" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same --glob-db='*506-unknown.jpeg'    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/    -dfR -T --min-same=10 --exclude-db=Picasa.ini    	- %s", "" );
  DUF_PRINTF( 0, "  run / --same-as-md5=/mnt/new_media/media/photo/Фото0829.jpg  -Rdfp    	- %s", "" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/  -Rpdf --same-md5 --glob-db='*506-unknown.jpeg'    	- %s", "" );
  DUF_PRINTF( 0, "  run --memusage  --nosccbs --allow-vacuum --flags    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150529.151033 ============" );
  DUF_PRINTF( 0, "  run --progress  -F     /mnt/new_media/media/photo/ --allow-remove-database -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif    	- %s", "" );
  DUF_PRINTF( 0, "  run --progress  --fast /mnt/new_media/media/photo/ --allow-remove-database -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif    	- %s", "" );

  DUF_PRINTF( 0, "========================= as for 20150603.145115 ============" );
  DUF_PRINTF( 0, "  run --mem --trace-temporary --max-dirfiles=17 --max-dirs=234 --max-files=543 --max-items=951 "
              " --fast  --min-dirfiles=13 --include-fs='*.jpg'  --include-fs='*.png' --exclude-fs='*.avi' --exclude-fs='*.mp4'    	- %s",
              "debug to show some data (temporary); argv-typed options --(in|ex)clude-fs=" );
  DUF_PRINTF( 0, "  run  --memusa --max-rel-depth=? --max-depth=? --output-level=5 --output-level=?    	- %s",
              "show values (? instead of value; esp. for interactive)" );

  DUF_PRINTF( 0, "  run  --open-db --cd=/mnt/new_media/media/photo/     -dfR -p   --md5id=857    	- %s", "" );
  DUF_PRINTF( 0, "  run                 /mnt/new_media/media/photo/     -dfR -p   --md5id=857    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150604.115535 ============" );
  DUF_PRINTF( 0, "  run -df --interac    	- %s", "\n	tree\n	recursive\n	tree\n	no-recursive\n	tree\n" );

  DUF_PRINTF( 0, "  run --allow-remove-database -O -P /mnt/new_media/media/photo/Pictures/one/    	- %s",
              "remove old and create new DB, add path to DB; -O, -P, --allow-remove-database are flags!" );
  DUF_PRINTF( 0, "  run --allow-remove-database /mnt/new_media/media/photo/Pictures/one/ -OP    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  /mnt/new_media/media/photo/Pictures/one/ -OP --allow-remove-database    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150806.154601 ============" );
  DUF_PRINTF( 0, "  ( echo open-db ; echo \"cd=`pwd`/..\" ; echo 'evaluate-sccb=tree' ) | run -dfR   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR < test.cmds    	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "     test.cmds:  	- %s  \n"
              "\t\topen-db\n" "\t\tcd=/home/mastar/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf\n"
              "\t\tevaluate-sccb=tree\n", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150807.095509 ============" );
  DUF_PRINTF( 0, "  MSH_DUF_OPTIONS=trace-temp=-3 run  -dfR @tist.cmd  `pwd`/human @test.cmds --tree @tost.cmd   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  MSH_DUF_OPTIONS=trace-temp=1 run @test.cmds   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  cd human && MSH_DUF_OPTIONS=trace-temp=1 run @../test.cmds"
              " --same-as=/home/mastar/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf/test/duf/20150602/out20150602.134103.test.1   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  cd human && MSH_DUF_OPTIONS=trace-temp=1 run @../test.cmds"
              " --same-as=../test/duf/20150602/out20150602.134103.test.1   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150809.163906 ============" );
  DUF_PRINTF( 0, "  rm 1.tmp ; run  @scan-here.cmds   /home/mastar/tmp . --output-file=1.tmp ; l 1.tmp   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150812.171929 ============" );
  DUF_PRINTF( 0, "  run  -dR --cd=human --tree .auxdir/ human/   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run @test.cmds  /   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  rm  $MSHDIRS_DIR/lib/duf_db/temp.db ; shn m i r  --trace-path=0 --trace-temp=0 --cd=.auxdir/ @scan-here.cmds  human   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0,
              "  run --db-name=photo.20150812.124329.db   -dfR --lsf "
              " --same-as=/home/mastar/big/misc/media/photo/dvd/benq/benq/saltov/imag0154.jpg /   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.20150812.124329.db -dfR --glob='*.avi' --min-same=2 --tree /   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150820.205622 ============" );
  DUF_PRINTF( 0, "  run --trace-explain=2 --memusage  -dfR --trace-fs=0  --trace-mod=2 " " --include-fs='.*.sw*'  --evaluate-sccb=dummy .   	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run   --trace-scan=0 --trace-fs=0  --allow-drop-tables  -dfR --max-depth=3 "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress .   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --disable-memusage  -dfR  --tree  --max-depth=2 .   	- %s", "new opt: --disable-memusage; --memusage removed" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db /home/mastar/big/misc/media/photo  "
              " -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif  " " --progress   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20150826.112235 ============" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --md5id=854 --disable-fs /home/mastar/big/misc/media/photo  --tree    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --md5id=854 --disable-fs /home/mastar/big/misc/media/photo  --tree "
              " --tag-file=ch-IMAG3115  --evaluate-sccb=tagit   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --with-tag-file=ch-IMAG3115 /home/mastar/big/misc/media/photo  --tree   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db -dfR  --files-tagged-as='ch-IMAG3115' /home/mastar/big/misc/media/photo --tree   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  /home/mastar/big/misc/media/photo/LG_G3/ "
              " --min-same=2 --tag-file=at-LG_G3  --evaluate-sccb=tagit   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' / --tree   	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run  --db-name=htc_20101224.db  --allow-drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/Pictures/HTC_Legend/20101224  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=lg_g3.db  --allow-drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/LG_G3/  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=htc.db  --allow-drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif --progress /home/mastar/big/misc/media/photo/Pictures/HTC_Legend/  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' --with-mime-type=application/octet-stream / --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0  --db-name=photo.db -dfR   --files-tagged-as='at-LG_G3' --files-tagged-as='ch-IMAG3115'  / --ls  	- %s",
              "allowed multiple --files-tagged-as options" );

  DUF_PRINTF( 0, "========================= as for 20150827.165852 ============" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='LG-D855'   /home/mastar/big/misc/media/photo --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='LG-'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='EZX'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='KODAK'   /home/mastar/big/misc/media/photo --ls  	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='C433'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --trace-sql=0 --db-name=photo.db -dfR   --with-camera='2110'   /home/mastar/big/misc/media/photo --ls  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150829.201454 ============" );
  DUF_PRINTF( 0, "  run  --db-name=photo.db --md5id=854  -dfR --use-format=2 /  --tree  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150830.125604 ============" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --min-size=10k --min-same=4 "
              " --with-mime-type=image/jpeg  --save-dir=/tmp --evaluate-sccb=save_to /  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --min-size=10k --min-same=4 "
              " --with-camera=2110  --save-dir=/tmp --evaluate-sccb=save_to --max-seq=30 /  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --min-size=10k --min-same=4 --with-camera=2110 "
              " --save-dir=/tmp/%%a/%%Tx{%%Y/%%B/%%d/%%H%%M%%S}/%%f --evaluate-sccb=save_to /  	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --with-mime-type=image/jpeg --with-camera=D855 "
              " --save-dir=/home/mastar/big/misc/media/photo/save/%%Tx{%%Y}/%%a/%%Tx{%%B/%%d/%%H%%M%%S}/%%f --evaluate-sccb=save_to /  	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "  run --db-name=photo.db -dfR  --trace-mod=2 --with-mime-type=image/jpeg --with-camera=2110 "
              " --save-dir=save/%%Tx{%%Y}/%%a/%%Tx{%%B/%%d/%%H%%M%%S}/%%M --evaluate-sccb=save_to /home/mastar/big/misc/media/photo  	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150902.181551 ============" );
  DUF_PRINTF( 0, "  run  --db-name=200105.db  --allow-drop-tables  -PO -fR  --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,mime,exif "
              " --progress  /home/mastar/big/misc/media/photo/dvd/dvd1/200105  	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=200105.db  -dfR  --with-mime=image/jpeg --ls /home/mastar/big/misc/media/photo/dvd/dvd1/200105  	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150908.155827 ============" );
  DUF_PRINTF( 0, "  run  --trace-path=1 -dfR --trace-options=2 --output-file=/dev/pts/30 --min-same=5 "
              " --cd=/home/mastar/big/ --tree /home/mastar/big/misc/develop/autotools/zoc/projects/ /home/mastar/big/misc 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  rm aaa ; run  --trace-file=aaa  --trace-path=0 -dfR --trace-options=2  --output-file=/dev/pts/30  --min-same=5 "
              " --cd=/home/mastar/big/misc/ --tree ; date > /dev/pts/30 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150909.120534 ============" );
  DUF_PRINTF( 0, "  export MSH_DUF_AOPTIONS='trace-options=3' 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  export MSH_DUF_ZOPTIONS='option-delimiter=3b' 	- %s", "-= \"hex(';')=3b\" =-" );
  DUF_PRINTF( 0, "  export MSH_DUF_OPTIONS='trace-path=1;trace-options=1' 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR --db-name=down.db --output-file=/dev/pts/30  --md5id=26284 --cd=/ --tree ; date > /dev/pts/3 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  echo 'tree:ls' | run -dfR --db-name=down.db --output-file=/dev/pts/30 --md5id=16288  --cd=/ ; date > /dev/pts/30 	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  echo ':;tree;ls' | run -dfR --db-name=down.db --output-file=/dev/pts/30 --md5id=16288  --cd=/ ; date > /dev/pts/30 	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  echo ':@tree@ls' | run -dfR --db-name=down.db --output-file=/dev/pts/30 --md5id=16288  --cd=/ ; date > /dev/pts/30 	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR --output-file=/dev/pts/30 --trace-db=2 --max-seq=24 --cd=/ --ls "
              " --echo=\"@1@@@@@@@@@@@@@@@@@@@@@@@\" --ls --cd=.. --echo=\"@2@@@@@@@@@@@@@@@@@@@@@@@\""
              " --ls  ; date > /dev/pts/30 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR --output-file=/dev/pts/30 --trace-db=2 --max-seq=24 --cd=/ --ls "
              " --echo=\"'@1@@@@@@@@@@@@@@@@@@@@@@@`date`'\" --ls --cd=.. "
              " --echo=\"'@2@@@@@@@@@@@@@@@@@@@@@@@`date`'\" --ls " " --echo=\"'@3@@@@@@@@@@@@@@@@@@@@@@@`date`'\" 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150916.095253 ============" );
  DUF_PRINTF( 0, "  run -dfR --filename=IMAG0691.jpg --db-name=photo --evaluate=tree  /	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR --dataid=11062 --db-name=photo --evaluate=tree  /	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -dfR --dataid=11063 --filename=IMAG0692.jpg --db-name=photo --evaluate=listing  /	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR --min-datasame=2 --evaluate=tree	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR --min-sizesame=2 --evaluate=tree	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR --min-mimesame=2 --evaluate=tree	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR --min-exifsame=2 --evaluate=tree	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150917.204538 ============ new: ...sha1" );
  DUF_PRINTF( 0, "  run test/tree/ --allow-drop-tables  -PO -dfR  --evaluate-sccb=sha1 --progress	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR  --evaluate=tree --same-as-md5=test/tree/t2.html 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run . -dfR  --evaluate=tree --same-as-sha1=test/tree/t2.html 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20150918.092133 ============" );
  DUF_PRINTF( 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" );
  DUF_PRINTF( 0, "  run --fast --trace-md5=0 --trace-sha=0 test/tree/ --allow-drop-tables  -PO -dfR "
              " --evaluate-sccb=dirs,filedata,filenames,sd5,sha,md5 --progress 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "[==========================] 100%%; 27 of 27; 0 to do; 0.155882 5.7734 ◁ file data ▷     	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 27 of 27; 0 to do; 0.138797 5.14062 ◁ file names ▷     	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 491 of 491; 0 to do; 0.121471 0.247395 ◁ collect sd5 ▷     	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 129 of 129; 0 to do; 0.0852289 0.660689 ◁ collect sha1 ▷    	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 127 of 127; 0 to do; 0.0862439 0.679086 ◁ collect md5 ▷   	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "2 same md5 different files included : 129 != 127	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" );
  DUF_PRINTF( 0, "  run --fast --db-name=photo.db  --allow-drop-tables  -PO -fR "
              " --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,sha1,mime,exif --progress /home/mastar/big/misc/media/photo	- %s",
              "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 1250 of 1250; 0 to do; 11.7811s avg:9.4249ms ◁ file data ▷       	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 1250 of 1250; 0 to do; 10.562s avg:8.44961ms ◁ file names ▷       	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 73504 of 73504; 0 to do; 175.329s avg:2.3853ms ◁ collect crc32 ▷   - %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 73504 of 73504; 0 to do; 13.6741s avg:0.186033ms ◁ collect sd5 ▷   - %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 70557 of 70557; 0 to do; 695.385s avg:9.85565ms ◁ collect md5 ▷    - %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 70288 of 70288; 0 to do; 690.397s avg:9.8224ms ◁ collect sha1 ▷    - %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 83474 of 83474; 0 to do; 617.98s avg:7.40327ms ◁ collect mime ▷    - %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "[==========================] 100%%; 54508 of 54508; 0 to do; 270.489s avg:4.96237ms ◁ collect exif ▷   - %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "shn:   exited with 0       	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "real 47m35.621s; user 8m38.055s; system 4m20.704s; cpu 27.27     	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" );

  DUF_PRINTF( 0, "========================= as for 20151006.194418 ============ new: --cmds-dir" );
  DUF_PRINTF( 0, "  ln -s `realpath dufconf/zocromas_duf.conf` /home/mastar/.mas/config/zocromas_duf.conf 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run @test0.cmds 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --config-dir=dufconf @test0.cmds 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --config-dir=/home/mastar/.mas/config  @test0.cmds 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --config-dir=/home/mastar/.mas/config/ --cmds-dir=test/duf/tests20151006  @test0.cmds 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151013.175104 ============ new: --allow-dir, --dir-priority, =set_dir_priority" );
  DUF_PRINTF( 0, "  run -du                      --db-name=test_tree  --trace-mod=7  --dir-priority=-6 "
              " --evaluate=set_dir_priority test/tree/ 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --allow-dir --allow-sub  --db-name=test_tree  --trace-mod=7  --dir-priority=-6 "
              " --evaluate=set_dir_priority test/tree/ 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run -d                       --db-name=test_tree  --trace-mod=7  --dir-priority=-6 "
              " --evaluate=set_dir_priority test/tree/ 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run --allow-dir              --db-name=test_tree  --trace-mod=7  --dir-priority=-6 "
              " --evaluate=set_dir_priority test/tree/ 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151015.101559 ============" );
  DUF_PRINTF( 0, "  run -du --allow-sub --no-allow-sub --db-name=test_tree  --trace-mod=7  --dir-priority=-6  --evaluate=tree test/tree/ 	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151019.111758 ============" );
  DUF_PRINTF( 0, "  MSH_SHN_DISABLE_MSG=yes MSH_SHN_DISABLE_MARKLINE=yes MSH_SHN_DISABLE_TIMING=yes run  --disable-memusage @test1.duf 	- %s",
              "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151022.150716 ============" );
  DUF_PRINTF( 0, "  run  --list-extended 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --list-extended=sccb 	- %s", "-= \"\" =-" );
  DUF_PRINTF( 0, "  run  --list-extended=sccb:ls 	- %s", "-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151026.121305 ============" );
  DUF_PRINTF( 0, "  run  -duR  --db-name=test_tree --cd-id=15      --evaluate=tree 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run  -duR  --db-name=test_tree --cd=test/tree  --evaluate=tree 	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151029.182731 ============" );
  DUF_PRINTF( 0,
              "  run -vv --trace-stdout --output-level=0 --trace-temp=1 --trace-sql=0 -dfuR --db-name=test_tree test/tree --use-format=3 --same-as-sha1=test/tree/test/duf/20150602/out20150602.131822.test.sql1 --tree  	- %s",
              " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151113.164253 ============" );
  DUF_PRINTF( 0, "  run     --std-leaf-set=3 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3  --ls test/tree/ -L 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run     --std-leaf-set=2 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3  --ls test/tree/ -L 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run     --std-leaf-set=2 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3  --ls test/tree/ -R 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run     --std-leaf-set=2 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3  --ls test/tree/ -Rdu 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run     --std-leaf-set=3 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3  --ls test/tree/ -uL 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run --output-level=0   --std-leaf-set=3 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3 "
              " --same-as-sha1=test/tree/test/duf/20150602/out20150602.131822.test.sql1 --ls test/tree/ -L 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0,
              "  run --output-level=0   --std-leaf-set=3 --trace-sccbh=0   -f --db-name=test_tree  --use-format=3 --sha1id=380 --ls test/tree/ -L 	- %s",
              " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20151115.165244 ============" );
  DUF_PRINTF( 0, "  run   --db-name=photo  --evaluate-sccb=listing /home/mastar/big/misc/media/photo  -f  --std-leaf-set=3 "
              " --force-color  -L | less -R 	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160105.200407 ============" );
  DUF_PRINTF( 0, "  run  --allow-drop-table --allow-create-tables --create-database test/tree/  test/tree/  --db-name=test_tree2 "
              " -dfuR  --evaluate-sccb=dirs,filedata,filenames,sd5,sha1,md5,crc32 2>&1 | less	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run     --interac --flags	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run  --allow-drop-table --allow-create-tables --create-database test/tree/  --db-name=test_tree2 "
              " -dfuR  --evaluate-sccb=dirs,filedata,filenames,sd5,sha1,md5,crc32 --progress 	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160107.144855 ============" );

  DUF_PRINTF( 0, "  run --help-set 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run --help-set=format  	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run --help-set=%%  	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160120.160108 ============" );
  DUF_PRINTF( 0, "  run  --allow-drop-table --allow-create-tables --create-database  --db-name=owncloud_mega_zip "
              " -dufR  --evaluate-sccb=dirs,filedata,filenames,sd5,sha1,md5,crc32 --progress "
              " /home/mastar/Desktop/20160108/Camera_Uploads "
              " /home/space/owncloud/data/mastarink/files/Camera "
              " /home/space/sambaspace/2016/y " " /home/space/sambaspace/2016/sdcard " " 	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run  --db-name=owncloud_mega_zip   -dufR  --evaluate-sccb=mime,exif --progress "
              " /home/mastar/Desktop/20160108/Camera_Uploads "
              " /home/space/owncloud/data/mastarink/files/Camera "
              " /home/space/sambaspace/2016/y " " /home/space/sambaspace/2016/sdcard " "  	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "  run --db-name=owncloud_mega_zip    -dRuf   --same-sha1=1  --evaluate-sccb=listing /  	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "** new: --linear " );
  DUF_PRINTF( 0, "  run  --db-name=owncloud_mega_zip --linear --std-leaf-set=4   -df  --evaluate-sccb=listing "
              " /home/space/sambaspace/2016/y/ " " --use-format=4  	- %s", " -= \"std-leaf-set=4\" =-" );

  DUF_PRINTF( 0, "  run  --db-name=owncloud_mega_zip --linear --std-leaf-set=4   -df  --evaluate-sccb=listing "
              " /home/ " " --use-format=4    	- %s", " -= \"same-groups almost shows (automatically)\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160130.174647 ============" );
  DUF_PRINTF( 0, "** new: commands from stdin " );
  DUF_PRINTF( 0, "  cat << EEE | run -df /home/"
              "												- %s\n	┊" "db-name=owncloud_mega_zip\n	┊"
              "linear\n	┊" "std-leaf-set=4\n	┊" "min-same-sha1=8\n	┊" "max-same-sha1=14\n	┊"
              "mimeid=1\n	┊" "with-camera=LG\n	┊" "evaluate-sccb=listing\n	┊" "use-format=4\n	┊\n" "	┊EEE", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160207.144135 ============" );
  DUF_PRINTF( 0, "** new: some test flags " );
  DUF_PRINTF( 0, "  run --no-test-act-flag --test-act-flag --test-act-noflag " " --test-recetc-flag --test-recetc-noflag "
              "--no-test-recetc-noflag --test-cfg-num=654321  	- %s", " -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160211.200000 ============" );
  DUF_PRINTF( 0, "** new: --fresh " );
  DUF_PRINTF( 0, "  run --fresh --allow-create-tables --allow-create-database --db-name=temp`datem` "
              " -dufR --evaluate-sccb=dirs,filedata,filenames,sd5  `pwd`/test/tree --progress  	- %s", " --fresh -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160212.131236 ============" );
  DUF_PRINTF( 0, "  run --db-name=test_tree -dufR --with-tag-file=test-gz --evaluate-sccb=tree `pwd`/test/tree  	- %s", " -= \"\" =-" );
  DUF_PRINTF( 0, "** --std-leaf-set =>  --std-leaf-set-num; new: --std-leaf-set-name " );
  DUF_PRINTF( 0, "  run --db-name=owncloud_mega_zip_copy --linear --std-leaf-set-num=4 --std-leaf-set-name=std-all-under "
              " -df --sha1id=1351 --tag-file=test-one --evaluate-sccb=listing,tagit  /home/ --use-format=4  	- %s",
              " --std-leaf-set-num=4 --std-leaf-set-name=std-all-under	 -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160213.185738 ============" );
  DUF_PRINTF( 0, "  run --test-act-bflag --test-act-nobflag --test-recetc-bflag --test-disable-nobflag --no-test-disable-nobflag	 -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160316.173058 ============" );
  DUF_PRINTF( 0, "  alias t0='run --trace-explain=8  --allow-drop-table --allow-create-tables  --allow-create-database "
              " --db-name=temp`datem`    -dufR   --evaluate-sccb=dirs,filedata,filenames,crc32,sd5,md5,sha1,mime,exif "
              " `pwd`/test/tree  `pwd`/test/tree    --progress'	 -= \"\" =-" );
  DUF_PRINTF( 0, "  alias t1='run   --db-name=owncloud_mega_zip_copy --linear  --std-leaf-set-name=std-all-under   -df --sha1id=1351 "
              " --tag-file=test-one  --evaluate-sccb=listing,tagit  /home/  --use-format=4'	 -= \"\" =-" );
  DUF_PRINTF( 0, "  alias t2='run  --db-name=test_tree  -dufR  --with-tag-file=test-gz  --evaluate-sccb=tree "
              " `pwd`/test --disable-memusage'	 -= \"\" =-" );
  DUF_PRINTF( 0, "  alias t3='run  --db-name=test_tree  -dufR --max-depth=17 --with-tag-file=test-gz1 "
              " --evaluate-sccb=listing+tree `pwd`/test'	 -= \"\" =-" );
  DUF_PRINTF( 0, "  alias t4='run --trace-temp=-20 --disable-updat --disable-in  --disable-fs  --flags'	 -= \"\" =-" );
  DUF_PRINTF( 0, "========================= as for 20160425.105024 ============" );
  DUF_PRINTF( 0, "  run  --db-name=owncloud_mega_zip_copy --linear --orderid=5  --std-leaf-set-name=std-all-under   -df "
              " --sha1id=1351  --tag-file=test-one  --evaluate-sccb=packlist  /home/  --use-format=4 --list-format-name=wow	 -= \"\" =-" );
  DUF_PRINTF( 0, "*** see human/alias.t.20160422.txt" );

  DUF_PRINTF( 0, "========================= as for 20160426.174114 ============" );
  DUF_PRINTF( 0, "  t__1p --list-format-name=4 --pack-format-name=wow	 -= \"\" =-" );
  DUF_PRINTF( 0, "*** see human/alias.t.20160422.txt :" );
  DUF_PRINTF( 0, "  alias t__1p='t__lreset && run  --db-name=owncloud_mega_zip_copy --linear "
              " --orderid=5  --std-leaf-set-name=std-all-under   -df --sha1id=1351  --tag-file=test-one "
              " --evaluate-sccb=packlist  /home/  --use-format=4'	 -= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160427.124739 ============" );
  DUF_PRINTF( 0, "  t__1p --format-name=sha	-= \"\" =-" );
  DUF_PRINTF( 0, "*** see human/alias.t.20160422.txt :" );
  DUF_PRINTF( 0, "  alias t__1pa0='run   --db-name=owncloud_mega_zip_copy --linear "
              " --min-same-sha1=5  --std-leaf-set-name=std-all-under  --glob=\"*.jpg\"  -df "
              "  --evaluate-sccb=packlist  /home/  --use-format=4  --trace-stdout --trace-force-color'	 -= \"\" =-" );
  DUF_PRINTF( 0, "  t__1pa0 --format-name=sha	-= \"\" =-" );

  DUF_PRINTF( 0, "========================= as for 20160510.111922 ============" );
  DUF_PRINTF( 0, "  run  --allow-drop-table --allow-create-tables --allow-create-database  --db-name=owncloud_mega_zip_pic "
              " -dufR  --evaluate-sccb=dirs,filedata,filenames,sd5,sha1,md5,crc32 --progress "
              " /home/mastar/Desktop/20160108/Camera_Uploads "
              " /home/mastar/.mas/lib/big/misc/media/photo/Pictures "
              " /home/mastar/.mas/lib/big/misc/media/photo/Pictures.R.20120207.164339/ "
              " /home/space/owncloud /data/mastarink/files/Camera "
              " /home/space/sambaspace/2016/y " " /home/space/sambaspace/2016/sdcard 	 -= \"\" =-" );
  DUF_PRINTF( 0, "=============================================================" );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_examples, void );
}
