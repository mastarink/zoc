#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
    cd $rootdir
    grep --color=yes -r --inc='*.ac' --inc='*.am' "$@" {commonlibs,bins,zoc*}
  fi
fi
