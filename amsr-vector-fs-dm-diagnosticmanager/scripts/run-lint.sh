#!/bin/bash

SCRIPT_DIR="$(dirname "$(readlink -f $0)")"
ROOT_DIR="$(dirname "${SCRIPT_DIR}")"
LINT_CMD="/usr/share/vector-rde-cpplint/infrastructure-cppstyle-guide/cpplint/cpplint.py"
FORMATTING_CMD="clang-format"
MODE="change"

using() {
  echo "Usage:" >&2
  echo "  $0 [-h] [-a] " >&2
  echo >&2
  echo "  run cpplint on all (changed) files" >&2
  echo >&2
  echo "  -a       lint all files instead of changed files" >&2
  echo "  -h       Print this help and exit" >&2
  echo >&2
  exit 1
}

#
# parsing of command line arguments
#
parseArguments() {
  while getopts "ah" opt; do
    case $opt in
      a)
        MODE="all"
        ;;
      h)
        using
        ;;
      :)
        ;;
    esac
  done
  shift $(($OPTIND - 1))
  REMAINING=("$@")
}

execute_lint() {
  ${LINT_CMD} $1 > /dev/null
  return $?
}

execute_clang_format() {
  DIFF=$(${FORMATTING_CMD} "$1" | diff "$1" --to-file=-)
  if [ "$DIFF" ]; then
    echo Formatting error for "$1"
    echo "$DIFF"
  fi
}

execute_checks() {
  execute_lint $1
  execute_clang_format $1
}

parseArguments $@
errors=0

if [ "${MODE}" == "all" ]; then
  echo "Linting all files..."
  for filename in $(find $ROOT_DIR/addon $ROOT_DIR/lib $ROOT_DIR/src $ROOT_DIR/test -\( -name "*.cc" -or -name "*.h" -\) 2> /dev/null); do
    execute_checks "$filename"
    errors=$(($errors+$?))
  done
else
  echo "Linting changes..."
  git diff --cached --name-only --diff-filter=ACM | while read file; do
    if [[ "$file" =~ \.(h|cc)$ ]]; then
      execute_checks "$ROOT_DIR/$file"
      errors=$(($errors+$?))
    fi
  done
fi

echo $errors Errors
exit $errors
