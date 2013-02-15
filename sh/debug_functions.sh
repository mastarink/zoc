function mas_debug ()
{
  local sedex cmdfile tmpcmd lt mcaller dname

  mcaller=$1
  shift
  dname=$( basename $mcaller )
  if [[ $dname =~ debug_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    dname="mas_$bname"
  elif [[ $dname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    dname="mas_$bname"
  fi
  echo "<< $dname >>">&2

  if [[ -d "$indir/src/$bname" ]] ; then
    srcdir="$indir/src/$bname"
  else
    srcdir="$indir/src"
  fi

  if [[ -f "$srcdir/.libs/$dname" ]] ; then
    binsdir="$srcdir/.libs"
    lt=yes
  else
    binsdir="$srcdir"
    unset lt
  fi
  libsdirs="$( find -type d -name '.libs' -printf '%p:' )"




# echo "mas_name: $mas_name" >&2
# echo "bname: $bname" >&2
# echo "dname: $dname" >&2
# echo "srcdir: $srcdir" >&2
# echo "binsdir: $binsdir" >&2


  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  ulimit -c

  cmdfile="$debugdir/debug_${dname}.cmd"
  sedex="s@^\(run\)@\1  $@@"

  tmpcmd="${cmdfile}.tmp"
  sed -e "$sedex" "$cmdfile" > $tmpcmd
  echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >&2
  

  if [[ "$lt" ]] ; then
    echo "[$mcaller] [$dname] [$bname] {$cmdfile} $binsdir/$dname -- $tmpcmd" >&2
    if [[ -f "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
  # libtool --mode=execute gdb -batch $binsdir/$mas_name -x $tmpcmd
      libtool --mode=execute gdb        $binsdir/$dname -x $tmpcmd
    else
      echo "no file : $cmdfile" >&2
    fi
  else
    echo "[$mcaller] [$dname] [$bname] {$cmdfile} $binsdir/$dname -- $tmpcmd" >&2
    gdb        $binsdir/$dname -x $tmpcmd
  fi
}
function gdb_core_any ()
{
  local mcaller dname bname srcdir binsdir libsdirs corename

  mcaller=$1
  shift
  dname=$( basename $mcaller )
  if [[ $dname =~ gdbcore_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    dname="mas_$bname"
  elif [[ $dname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    dname="mas_$bname"
  fi
  echo "<< $dname >>">&2

  if [[ -d "$indir/src/$bname" ]] ; then
    srcdir="$indir/src/$bname"
  else
    srcdir="$indir/src"
  fi

  if [[ -f "$srcdir/.libs/$dname" ]] ; then
    binsdir="$srcdir/.libs"
    lt=yes
  else
    binsdir="$srcdir"
    unset lt
  fi
  libsdirs="$( find -type d -name '.libs' -printf '%p:' )"
# srcdir="$indir/src/$bname"
# binsdir="$srcdir/.libs"
# libsdirs="$( find -type d -name '.libs' -printf '%p ' )"

# echo "mas_name: $mas_name" >&2
# echo "bname: $bname" >&2
# echo "dname: $dname" >&2
# echo "srcdir: $srcdir" >&2
# echo "binsdir: $binsdir" >&2


  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  ulimit -c

  corename=$( ls -1tr /tmp/core.$UID.$UID.${dname}.* | tail -1 )
  echo "dname: {$dname} ; core:{$corename}" >&2
  if [[ -f "$corename" ]] ; then
    libtool --mode=execute gdb $binsdir/$dname -c "$corename"
  fi
}
function debug_any ()
{
#   clear && 
  make_any && mas_debug $@
}








