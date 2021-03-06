# unalias cp
function zoc_error ()
{
  local ln fn
  ln=$1
  shift
  fn=$1
  shift
  echo "$ln $fn ERROR $@" >&2
}
. ${MAS_PROJECTS_DIR:-.}/sh/libdir_functions.bash
. ${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash
function prjconfV ()
{
  if [[ -f "./configure" ]] ; then
    ./configure -V
  elif [[ -f "./config.status" ]] ; then
    ./config.status -V
  elif [[ "$build_at" ]] && [[ -f "$build_at/config.status" ]] ; then
    $build_at/config.status -V
  fi
}
function prjconffullversion ()
{
  prjconfV | head -1
}
function prjconfname ()
{
  prjconffullversion | awk '{print $1}'
}
function prjconfversion ()
{
  prjconffullversion | awk '{print $3}'
}  
function setup_vers ()
{
  local n v rprefix rname_case c tmprex
  local global_flavour_opts_file indir_flavour_opts_file
  n=$1
  shift
  v=$1
  shift
  projectsfile=$projectsdir/projects.list
  if [[ -f $projectsfile ]] ; then
    projects_list=`cat $projectsfile`
#   readarray projects_list_array < $projectsfile
#   echo "[>>>> ${projects_list_array[2]} <<<<]" >&2
  else
    echo "not exists projectsfile: '$projectsfile'" >&2
    return 1
  fi
  if ! [[ "$n" ]] ; then n=`prjconfname`    ; fi
  if ! [[ "$n" ]] ; then
    n=`basename $(pwd)`
    if ! [[ "$n" ]] || ! { echo $projects_list | grep -sc "\<$n\>" &>/dev/null ; } ; then
      echo "ERROR (SV)>> [$n] . [$v] {$projects_list}" >&2
      unset n
      return 1
    fi
  fi
  if ! [[ "$v" ]] ; then v=`prjconfversion` ; fi
  if ! [[ "$v" ]] ; then v='unknown' ; fi
  if [[ "$n" ]] && [[ "$v" ]] ; then
#   echo "[$( basename $0 )] SET name:$n; version:$v" >&2
    if [[ -f "$indir/configure.ac" ]] ; then
      if [[ "$tworkunpack" ]] && [[ -d "$tworkunpack" ]] && [[ "$tworkbuild" ]] && [[ -d "$tworkbuild" ]] \
      			&& [[ "$savedirdist" ]] && [[ -d "$savedirdist" ]] ; then
	unpackdir="$tworkunpack/${n}-${v}"
	ibuilddir="$tworkbuild/${n}"
	distname="${n}-${v}.tar.bz2"
	distfile="$savedirdist/$distname"
	distfile_prefix="$savedirdist/${n}"
      fi
      wbuilddir="$inbuilddir"
      build_at="$wbuilddir"
      configuredir=$indir
      mas_name=$n
      mas_vers=$v
#     echo "`pwd`>> [$mas_name] [$mas_vers]" >&2
      mas_base_vers='0.0.5.20130219'
    fi
    global_flavour_opts_file=$projectsdir/configure_opts.${mas_flavour:-default}
    if ! [[ -f $global_flavour_opts_file ]] ; then
      echo "no file $global_flavour_opts_file" >&2
      echo "          using $projectsdir/configure_opts.default" >&2
      global_flavour_opts_file=$projectsdir/configure_opts.default
    fi
    indir_flavour_opts_file=$indir/configure_opts.${mas_flavour:-default}
    if ! [[ -f $indir_flavour_opts_file ]] && [[ -f $indir/configure_opts.default ]]; then
      echo "no file $indir_flavour_opts_file" >&2
      echo "          using $indir/configure_opts.default" >&2
      indir_flavour_opts_file=$indir/configure_opts.default
    fi

    if ! [[ "$prj_configure_opts" ]] || [[ -f "$indir_flavour_opts_file" ]] ; then
      prj_configure_opts="--prefix=$flavourdir"
  #   prj_configure_opts=" --silent --enable-silent-rules --enable-tracemem --enable-debug"
  #   prj_configure_opts="$prj_configure_opts --with-base-dir=/mnt/new_misc/develop/autotools/zoc"
  #   prj_configure_opts="$prj_configure_opts --with-pids-dir=zocromas/pid"
  #   prj_configure_opts="$prj_configure_opts --with-mods-dir=zocmds"
  #   prj_configure_opts="$prj_configure_opts --with-proto-dir=zocromas_zocprotos"
  #   prj_configure_opts="$prj_configure_opts --with-log-dir=log"
  #   prj_configure_opts="$prj_configure_opts --with-server=$MAS_PROJECTS_TMPDIR/zocromas.socket"
  #   prj_configure_opts="$prj_configure_opts --with-def-proto=xcromas"
      if [[ "$MAS_SH_VERBOSE" ]] ; then echo "to read '$global_flavour_opts_file'" >&2 ; fi
      tmprex='^#'
      while read c ; do 
	if ! [[ "$c" =~ ^--prefix= ]] &&  ! [[ "$c" =~ $tmprex ]] ; then
	  prj_configure_opts="$prj_configure_opts $c"
	fi
      done < $global_flavour_opts_file
      if [[ -f $indir_flavour_opts_file ]] ; then
        if [[ "$MAS_SH_VERBOSE" ]] ; then echo "to read '$indir_flavour_opts_file'" >&2 ; fi
	while read c ; do
	  if ! [[ "$c" =~ ^--prefix= ]] && ! [[ "$c" =~ $tmprex ]] ; then
	    prj_configure_opts="$prj_configure_opts $c"
	  fi
	done < $indir_flavour_opts_file
      else
        if [[ "$MAS_SH_VERBOSE" ]] ; then  echo "did not read $indir_flavour_opts_file" >&2 ; fi
      fi
    else
      if [[ "$MAS_SH_VERBOSE" ]] ; then echo "preset opts" >&2 ; fi
    fi
    instshname="$instshdir/${mas_name}-${mas_vers}.sh"
    mas_fullname="${mas_name}-${mas_vers}"
  else
    echo "n:$n v:$v" >&2
    echo "ERROR $LINENO setup_vers : '$mas_name'" >&2
    return 1
  fi
# echo "[$mas_name] [$mas_vers]" >&2
# ebuild_prefix=zocromas_
  ebuild_dir=$projectsdir/ebuilds/mas-tar/${ebuild_prefix}${mas_name}

  rprefix=$binprefix
 
  mcaller_preset=$0
  mcaller_fname=$( basename $0 )
  if [[ $mcaller_fname =~ (run|debug|gdbcore)_(${rprefix}.*)\.sh$ ]] ; then
    rname_preset=${BASH_REMATCH[2]}
    rname_case=1
  elif [[ $mcaller_fname =~ (run|debug|gdbcore)_(.*)\.sh$ ]] ; then
    rname_preset=${BASH_REMATCH[2]}
    rname_case=1
  elif [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    rname_preset=$MAS_ZOCROMAS_HERE
    rname_case=2
  fi
# echo "mcaller_fname:$mcaller_fname; rname_preset:$rname_preset; mas_name:$mas_name; rname_case:$rname_case; binary_preset:$binary_preset;">&2
  if [[ "$rname_preset" ]] && binary_preset="$bsrcdir/$rname_preset"           && [[ -f $binary_preset ]] && [[ -x $binary_preset ]] ; then
    rname_case=3
  elif binary_preset="$bsrcdir/${rprefix}${prjname}"  && [[ -f $binary_preset ]] && [[ -x $binary_preset ]] ; then
    rname_preset=$prjname
    rname_case=4
  elif binary_preset="$bsrcdir/${rprefix}${mas_name}" && [[ -f $binary_preset ]] && [[ -x $binary_preset ]] ; then
    rname_preset=$mas_name
    rname_case=5
  elif binary_preset="$bsrcdir/${mas_name}"           && [[ -f $binary_preset ]] && [[ -x $binary_preset ]] ; then
    rname_preset=$mas_name
    rname_case=6
    rprefix=''
  elif binary_preset="$bsrcdir/mtest"           && [[ -f $binary_preset ]] && [[ -x $binary_preset ]] ; then
    rname_preset=mtest
    rname_case=7
    rprefix=''
# else
#   echo "ERROR : prjname=$prjname ; no x: $build_at/src/[${rprefix}]${prjname}" >&2
#   echo "ERROR : mas_name=$mas_name ; no x: $build_at/src/[${rprefix}]${mas_name}" >&2
  else
    if [[ "$MAS_SH_VERBOSE" ]] ; then echo "can't set binary_preset @ $bsrcdir" >&2 ; fi
  fi
# echo "mcaller_fname:$mcaller_fname; rname_preset:$rname_preset; mas_name:$mas_name; rname_case:$rname_case; binary_preset:$binary_preset;">&2
  if [[ "$MAS_SH_VERBOSE" ]] ; then echo "($rname_case)::: $rname_preset" >&2 ; fi
# echo "[: $mas_name : $rprefix : $mcaller_fname : $rname_preset :]" >&2
  ibinary_preset="$flavourdir/bin/$rname_preset"
  if [[ "${binprefix}" ]] && [[ "$rname_preset" =~ ^${binprefix}(.+)$ ]] ; then
    short_name=${BASH_REMATCH[1]}
# else
#   echo "$rname_preset ::: ${binprefix} :: ---" >&2
  fi
# echo "binary_preset:$binary_preset" >&2
  if [[ "$binary_preset" ]] && [[ -f "$binary_preset" ]] ; then
    if [[ $(realpath --version 2>&1 | head -1 | awk '{print $4}') > 8 ]] && rbinary_preset=$( realpath --relative-to=$indir $binary_preset ) ; then
      :
    else
      rbinary_preset=$( realpath $binary_preset ) || return 1
    fi
  else
    if [[ "$MAS_SH_VERBOSE" ]] ; then echo "no binary_preset: $binary_preset [rname_preset: $rname_preset]" >&2 ; fi
  fi
# echo "binary_preset: $binary_preset" >&2
# echo "rbinary_preset: $rbinary_preset" >&2
# echo "build_at: $build_at" >&2
# echo "bsrcdir: $bsrcdir" >&2
# echo "binary_preset: $binary_preset" >&2
# echo "mcaller_preset: $mcaller_preset" >&2
# echo "rbinary_preset: $rbinary_preset" >&2
# echo "rname_preset: $rname_preset" >&2
# echo "binprefix: $binprefix" >&2
# echo "short_name: $short_name" >&2
  if ! [[ "$mas_name" ]] ; then return 1 ; fi
  return 0
}
function show_setup ()
{
  echo "wd:		$wd" >&2
  echo "0:		$0" >&2
  echo "me:		$me" >&2
  echo "mer:		$mer" >&2
  echo "shdir:		$shdir" >&2
  echo "shdirup:	$shdirup" >&2
  echo "indir:		$indir" >&2
  echo "configuredir:	$configuredir" >&2
  echo "wbuilddir:	$wbuilddir" >&2
  
  echo "= SAVE:" >&2
  echo "savedir:	$savedir" >&2
  echo "savedirdist:	$savedirdist" >&2
  echo "savedirtar:	$savedirtar" >&2
  echo "savedirtarme:	$savedirtarme" >&2
  
  echo "= TMP:" >&2
  echo "tworkdir:	$tworkdir" >&2
  echo "tworkunpack:	$tworkunpack" >&2
  echo "tworkbuild:	$tworkbuild" >&2
  
  echo "= INSTALL:" >&2
  echo "instdir:	$instdir" >&2
  echo "flavourdir:	$flavourdir" >&2
  echo "unpackdir:	$unpackdir" >&2
  echo "ibuilddir:	$ibuilddir" >&2
  echo "distname:	$distname" >&2
  echo "distfile:	$distfile" >&2

  echo "= NAME / VER:" >&2
  echo "mas_name:	$mas_name" >&2
  echo "mas_vers:	$mas_vers" >&2

  echo "= ROOT:" >&2
  echo "projectsdir:	$projectsdir" >&2
  
}
function virgin_clean ()
{
  echo "virgin" >&2
  for f in aclocal.m4 configure vrb.tmp ; do
    if [[ -f "$f" ]] ; then
      rm $f
    fi
  done
  find -name Makefile.in -exec rm \{} \;
  find -name '*.viminfo' -exec rm \{} \;
  for f in autom4te.cache .auxdir ; do
    if [[ -d "$f" ]] ; then
      rm -Rf $f
    fi
  done
# for f in zocversion.txt vimrc-mastar sh gvim-vimenter.vim gvimrc-mastar gvim-funcs.vim ; do
#   if [[ -L "$f" ]] ; then
#     echo "rm $f"
#   fi
# done
  for f in zocversion.txt vimrc-mastar.vim gvim-vimenter.vim gvimrc-mastar.vim gvim-funcs.vim ; do
    if [[ "$projectsdir" ]] && ! [[ -L "$f" ]]  ; then
      if [[ -f "$projectsdir/files/$f" ]] ; then
        ln -s `realpath --relative-to=. $projectsdir/files/$f`
      fi
    fi
  done
  for f in sh ; do
    if [[ "$projectsdir" ]] &&  ! [[ -L "$f" ]]  ; then
      if [[ -d "$projectsdir/$f" ]] ; then
        ln -s `realpath --relative-to=. $projectsdir/files/$f`
      fi
    fi
  done  
}
# if [[ "${MAS_PROJECTS_DIR}" ]] ; then cd "${MAS_PROJECTS_DIR}" ; fi
export MAS_MAKE_CNT=0
echo "loading setupz" >&2
if ! setup_dirs ; then
  echo "setup_dirs error" >&2
  if ! [[ "$0" == `which bash` ]] ; then
    echo "@@@@ $0 != `which bash` @@@@" >&2
#   exit
  fi
fi
# echo "/SETUPZ" >&2
