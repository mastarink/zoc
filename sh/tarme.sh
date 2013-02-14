#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  
  # show_setup
  function tarme ()
  {
    if [[ "$savedirtarme" ]] &&  [[ -d "$savedirtarme" ]] && [[ "$updir" ]] && [[ -d "$updir" ]] && [[ "$indir" ]] && [[ -d "$indir" ]] ; then
      if [[ "$mas_name" ]] && [[ "$mas_ver" ]] ; then
	cd $indir || return 1
	make -s distclean     || return 1  
	cd $updir  || return 1
	if [[ -d "$indir/log" ]] ; then
	  echo "rm $indir/log/server.*.log" >&2
	  echo "rm $indir/log/client.*.log" >&2
	  rm -f $indir/log/server.*.log
	  rm -f $indir/log/client.*.log
	  rm -f $indir/errors/make.*.result
	fi     
    #   echo "Saving $indir/*.{bz2,gz} to $savedirtar" >&2
    #   mv $indir/*.{bz2,gz} "$savedirtar"
	cd $updir  || return 1
	if [[ "$mas_name" ]] && [[ -d "$mas_name" ]] ; then
	  echo "Saving *.tar.{bz2,gz} to $savedirtarme" >&2
	  tar --wildcards --exclude '.*.sw*' --exclude-caches --exclude-tag mas-tar-exclude -jvcf "$savedirtarme/${mas_name}.${mas_ver}.`datemt`.tar.bz2" $mas_name/  || return 1
	fi
    #     mv *.tar.bz2 "$savedirtarme"     
	echo "Change to $indir to rebuild" >&2
	if cd "$indir" ; then
	  sh/autoreconf.sh || return 1
	  sh/configure.sh || return 1
	  sh/make.sh || return 1
	else
	  echo "Error change to $indir" >&2
	  return 1
	fi     
      else
	echo "No mas_name / mas_ver" >&2
	return 1
      fi    
    fi
    return 0
  }
  tarme $@
fi
