function make_target ()
{
  local ilog target
  target=$1
  shift
# echo "install [$prjname]" >&2
  ilog="/tmp/$target-$prjname.log"
  if make -s $target > $ilog ; then
    echo "$prjname $target OK : $ilog" >&2
  else
    echo "$prjname $target FAIL" >&2
    cat "$ilog" >&2
    return 1
  fi
  return 0
}
function make_m ()
{
  local ername erdir
  erdir='errors'
  ername="$erdir/make.`datemt`.result"
  if [[ "$erdir" ]] && ! [[ -d "$erdir" ]] ; then
    mkdir $erdir
  fi
  if [[ "$ername" ]] && ! [[ -f "$ername" ]] ; then
    if make -s $@ >$ername 2>&1 ; then
      rm $ername
#     echo "$LINENO GOOD make_m" >&2
      return 0
    elif [[ -s "$ername" ]] ; then
      echo -n "$ername >>>>>>>>" >&2
      if false ; then
	cat $ername
      else
	grep -v '^Making all in ' $ername >&2
      fi
      echo -n "<<<<<<<<" >&2
    # grep error $ername | head -1 | sed -ne 's/^\([a-z]\+\.[ch]\):\([[:digit:]]\+\):.*$/\1 :: \2/p'
    else
      echo "$LINENO ERROR make_m" >&2
    fi
  else
    echo "$LINENO ERROR make_m" >&2
  fi
  return 1
}
function grep_make_m ()
{
  if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
    cd $indir
  # echo "grep_make_m" >&2
  # grep  --include='*.[ch]' -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src inc | grep -v '^src/mas_lib[0-9]*\.c'
    if [[ -f "grepfunexclude.txt" ]] ; then
    ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src && \
    ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(malloc\|free\|strdup\|strndup\)\>\s*(' src && \
      make_m
    else
      make_m
    fi
  # make -s ctags
  fi
}
function make_any ()
{
# echo "make_any" >&2
  if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
    cd $indir
    time grep_make_m
  else
    echo "$LINENO ERROR : no dir $indir" >&2
#    exit
  fi
}
function make_any_tilme ()
{
  local timname tstart
  timname=make-timing.`datemt`.result
  if [[ "$timname" ]] ; then
    if ! [[ -f "$timname" ]] ; then
      tstart=`date`
      if make_any $@ > "$timname" 2>&1 ; then
#   pwd >&2
#   echo "timname:$timname" >&2
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> makes.log
	cat "$timname"
	rm "$timname"
      else
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> makes.log
	cat "$timname"
	rm "$timname"
	return 1
      fi
    fi
  fi
  return 0
}

