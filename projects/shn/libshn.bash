function shn_code ()
{
  shn_dbgmsg "-=<shn_code>=-"
  local retcode=0
  local i
  declare -gx -A MSH_SHN_LAST_ACTION
  local code=${1:-h}
  shift
  local codeopts

  shn_dbgmsg "shn 1 -- $code"
  shn_dbgmsg "shn 2 -- $code"
# for (( i=1; i <= $# ; i++ )) ; do echo "$FUNCNAME $i : ${!i}" >&2 ; done

  local nocase=u
  if shopt  nocasematch &>/dev/null ; then nocase=s ; else nocase=u ; fi
  shopt -u nocasematch &>/dev/null
  if [[ "$MSH_RUN_MODE_REPLACE" ]] && [[ "$code" == r ]] ; then
    code=$MSH_RUN_MODE_REPLACE
  fi
# shn_msg "shn pwd:`pwd`"
  case $code in
    h)
      shn_msg 'project' $MSH_SHN_PROJECT_NAME
      shn_msg 'h = help'
      shn_msg 'r = run'
      shn_msg 'g = debug'
      shn_msg 'G = core debug'
      shn_msg 'l = list'
      shn_msg 'j = jum project'
      shn_msg 'c = configure'
      shn_msg 'C = make clean'
      shn_msg 'D = make distclean'
      shn_msg 'U = "super" clean'
      shn_msg 'a = autoreconf'
      shn_msg 't = make distcheck and save tarballs to' ${MSH_SHN_DIRS[savedist]}
      shn_msg 'm = make'
      shn_msg 'i = make install'
      shn_msg 'u = make uninstall'
      shn_msg 'E = check files for e'
      shn_msg 'R = reload working shell libs (shn)'
      shn_msg 'L = list build dir'
      shn_msg "I = info for $MSH_SHN_PROJECT_NAME"
      shn_msg 'x = command at build dir'
      shn_msg 'e = ebuild - prepare distfiles and Manifest for Gentoo / emerge'
    ;;
    l)
      # pwd >&2 || return $?
####  shn_fmsg  "[--%02d----------- %30s ------------- %s]\n" ${project_index:-0} $MSH_SHN_PROJECT_NAME `datemt`
#     declare -p MSH_SHN_LAST_ACTION >&2
      MSH_SHN_LAST_ACTION[$MSH_SHN_PROJECT_NAME:list]=`datemt`
      shn_fmsg  "	[ %-30s ]" $MSH_SHN_PROJECT_NAME
    ;;
    j)
      shn_project_cd $1 || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shift
    ;;
    r)
      tmux send-keys -R  \; clear-history
      shn_run "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ; }
    ;;
    g)
      shn_uni_debug "-" "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
    ;;
    G)
      shn_uni_debug core "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
    ;;
    Y)
      shn_core_debug_installed "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
    ;;
    c)
      while [[ $1 == -* ]] ; do
	codeopts="${codeopts:+ }$1"
	let in_shift++
	shift
      done
      shn_msg "opts: [$codeopts]"
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_configure $codeopts || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    C)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_clean || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    D)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_distclean || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    U)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_superclean || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    UU)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_superclean wash || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;   }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    a)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_autoreconf || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;	  
    e)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_ebuild_check
      shn_build_ebuild_update    || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    E)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_ebuild_check    || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;	  
    t)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_dist      || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    m)
      shn_dbgmsg "shn 2.${code}.1"
      shn_build_make      || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.2"
    ;;
    I)
      if [[ "${MSH_SHN_DIRS[build]}" ]] && [[ -d "${MSH_SHN_DIRS[build]}" ]] && [[ -x "${MSH_SHN_DIRS[build]}/config.status" ]] ; then
        ${MSH_SHN_DIRS[build]}/config.status -V
      fi
    ;;
    i)
      shn_dbgmsg "shn 2.${code}.1"
#	  shn_build_make      || return $?
#         shn_dbgmsg "shn 2.${code}.2"
      shn_build_install || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.3"
    ;;
    u)
      shn_dbgmsg "shn 2.${code}.1"
#	  shn_build_make      || return $?
#         shn_dbgmsg "shn 2.${code}.2"
      shn_build_uninstall || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
      shn_dbgmsg shn $code ok
      shn_dbgmsg "shn 2.${code}.3"
    ;;    
    R)
      shn_load
    ;;
    L)
      shn_msg List
      shn_build_list "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
    ;;
    x)
      shn_std_command build "$@" || { retcode=$? ; shn_errmsg "shn ${code} r:$retcode" ;  }
    ;;
    q1)
      shn_msglist $@
    ;;
    q2)
      shn_msglist "$@"
    ;;
    q3)
      shn_msglist $*
    ;;
    q4)
      shn_msglist "$*"
    ;;
    q5)
      echo "[$*]"
      echo "[`cat /proc/$$/cmdline | tr '\0' '@'`]"
      echo "$# args : '$1' '$2' '$3' '$4' '$5' '$6' '$7' '$8' '$9'"
      return 1
    ;;
    *)
      shn_errmsg "wrong code '$code'"
      return 1
    ;;
  esac
  shn_dbgmsg "shn 3 -- $code"
  shn_dbgmsg "shn 4 -- $code ; next:$1"
  shopt -$nocase nocasematch &>/dev/null
  shn_dbgmsg "-=</shn_code>=-"
  return $retcode
}
function shn_control_c ()
{
  interrupted=$(( $interrupted +1 ))
  shn_msg " -----» Ctrl-C «-----"
  sleep 1
# trap - INT
}
function shn_i ()
{
  local retcode=0 interrupted=0
  local code=${1:-l} i
  local in_shift=0
  shift
  shn_dbgmsg "-=<shn_i>=-"
  shn_dbgmsg 1 shn
  shn_dbgmsg 2a shn
#?trap shn_exit EXIT
  shn_dbgmsg "project $MSH_SHN_PROJECT_NAME"
# shn_setup_projects || shn_project_cd "${MSH_SHN_PROJECT_NAME:-zoctypes}" || { retcode=$? ; shn_errmsg shn setup ; return $retcode ; }
  shn_setup_projects || shn_project_cd                                     || { retcode=$? ; shn_errmsg shn_i rc:$retcode ; return $retcode ; }
  shn_dbgmsg 3 shn
# shn_msg ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
  if [[ "$code" == each ]] || [[ "$code" == '..' ]] ; then
#   shn_msg "Will install to ${MSH_SHN_DIRS[flavour]}"
    shn_project_each '' 0 shn "$@" || { retcode=$? ; shn_errmsg "shn_i r:$retcode" ; return $retcode ; }
  elif [[ "$code" =~ ^\?(.*)$ ]] ; then
#   shn_msg "Will install to ${MSH_SHN_DIRS[flavour]}"
    shn_project_each "${BASH_REMATCH[1]}" 0 shn "$@" || { retcode=$? ; shn_errmsg "shn_i r:$retcode" ; return $retcode ; }
  elif [[ "$code" == cont ]] ; then
    if [[ "${MSH_SHN_DIRS[status]}" ]] && [[ -d "${MSH_SHN_DIRS[status]}" ]] ; then
      shn_msg ">>> $MSH_SHN_STATUS @ <<<"
#     if [[ -f "${MSH_SHN_DIRS[status]}/last" ]] ; then
#       read ifr dshn acts < "${MSH_SHN_DIRS[status]}/last"
#       shn_msg "@@@@@@@@@@@ $ifr -- $dshn -- $acts"
#       shn_project_each $ifr '' $dshn $acts
#     fi
      if [[ "$MSH_SHN_STATUS" ]] ; then
        shn_project_each '' $MSH_SHN_STATUS || { retcode=$? ; shn_errmsg "shn_i r:$retcode" ; return $retcode ; }
      fi
    fi   
  elif [[ "$code" == one ]] ; then
    local shn_dont_setup=yes
    
#   local shn_ignore_error=yes
    in_shift=0
    shn_code "$@" || { retcode=$? ; shn_errmsg "shn_i r:$retcode" ; return $retcode ; }
    shift $in_shift
    retcode=$?
  elif [[ "$code" ]] ; then
    local shn_dont_setup=yes
    
#   local shn_ignore_error=yes
    while [[ "$code" ]] ; do
      shn_dbgmsg "code:$code"
#     if [[ "$code" =~ ^(L|X|x|r)$ ]] ; then
#       shn_warn "use '$FUNCNAME one $code'"
#     else
        in_shift=0
        shn_code $code "$@" || { retcode=$? ; shn_errmsg "shn_i r:$retcode" ; return $retcode ; }
	shift $in_shift
#     fi
      if [[ "$code" =~ ^[LXxrgGj]$ ]] ; then
        return 0
      fi
      code=$1
      shift
    done
  else
    local shn_dont_setup=yes
    
#   local shn_ignore_error=yes
    shn_code h || { retcode=$? ; shn_errmsg "shn setup r:$retcode" ; return $retcode ; }
  fi
# shn_msg "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
  shn_dbgmsg "shn 5 -- $code"
  shn_dbgmsg shn "  <`datemt`> end($retcode)" -- ${MSH_SHN_PROJECT_NAME}
# shn_pwd
  shn_setup_projects || return $?
# shn_msg END of shn
  shn_dbgmsg "-=</shn_i>=-"
  return $retcode
}
function shn ()
{
  local TIMEFORMAT=${TIMEFORMAT:-real %lR; user %lU; system %lS; cpu %P}
  local -i retcode=0
  declare -gx MSH_SHN_CWD 
  shn_dbgmsg "-=<shn>=-"
  MSH_SHN_CWD=`pwd`
  shn_dbgmsg "MSH_SHN_CWD: $MSH_SHN_CWD" >&2
  trap shn_control_c INT
  if pushd . &>/dev/null ; then
    if [[ $MSH_SHN_DISABLE_TIMING ]] ; then 
      shn_i "$@"
      retcode=$?
    else
      time shn_i "$@"
      retcode=$?
    fi
    popd &>/dev/null
  fi
  trap - INT
  shn_dbgmsg "-=</shn>=-"
  return $retcode
}
