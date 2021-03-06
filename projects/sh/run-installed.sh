#!/bin/sh
if [[ -f "sh/librun_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/librun_functions.bash

  if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    run_installed ${MAS_ZOCROMAS_HERE} "$@"
  else
    echo "Pleasem set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
fi

