#!/bin/sh
if [[ -f "sh/build_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh
 
# echo "==================================================" >&2
# echo "distname: $distname" >&2
# echo "distfile: $distfile" >&2
# echo "ibuilddir: $ibuilddir" >&2
# echo "workdir: $workdir" >&2
# echo "configuredir: $configuredir" >&2
# echo "==================================================" >&2
# echo ">>> $@ <<<" >&2

  install_z $@
  
# echo "==================================================" >&2
# echo "distname: $distname" >&2
# echo "distfile: $distfile" >&2
# echo "ibuilddir: $ibuilddir" >&2
# echo "workdir: $workdir" >&2
# echo "configuredir: $configuredir" >&2
# echo "==================================================" >&2

fi