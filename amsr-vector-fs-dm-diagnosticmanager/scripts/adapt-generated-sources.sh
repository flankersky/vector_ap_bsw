#!/bin/bash
set -e
trap onEXIT EXIT

# Default values
DM_SRC_PATH=""

# ---------------------------------------------------------------------------
# Small helpers
#

absPath() {
  echo $(/bin/readlink -e "$1")
}

verbose() {
  if [ "$VERBOSE" = "true" ]
  then
    echo "$1"
  fi
}

checkIfFileExists() {
  local MSG=$1
  local FILE=$2
  verbose "Checking if file exists: $FILE"
  if [ ! -f "$FILE" ]
  then
    usage "$MSG"
  fi
}

checkIfDirExists() {
  local MSG=$1
  local DIR=$2
  verbose "Checking if directory exists: $DIR"
  if [ ! -d "$DIR" ]
  then
    usage "$MSG"
  fi
}

checkIfNotEmpty() {
  local MSG=$1
  local VALUE=$2
  if [ "$VALUE" = "" ]
  then
    usage "$MSG"
  fi
}

checkIfToolExists() {
  local TOOL=$1
  verbose "Checking if tool exists: $TOOL"
  if ! which "$TOOL" >/dev/null
  then
    echo "The tool '$TOOL' doesn't exists - please install it first"
    exit 1
  fi
}

# ---------------------------------------------------------------------------
# Exit handler
#

onEXIT()
{
  if [ "$?" != "0" ]
  then
    echo "$0 FAILED"
  fi
}

# ---------------------------------------------------------------------------
# Command line usage
#
usage() {
  local MSG=$1
  echo >&2
  if [ "$MSG" != "" ]
  then
    echo "ERROR: $MSG" >&2
    echo >&2
  fi
  echo "Usage:" >&2
  echo "  $0 [-h] [-p <dm-sources-path>] " >&2
  echo >&2
  echo "Adapt the generated sources with correct clang format and #ifndef MACROs" >&2
  echo >&2
  echo "Options:" >&2
  echo "  -p                    Path to the source directory of diagnostic manager." >&2
  echo >&2
  echo "  -h                    Print this help and exit" >&2
  echo >&2
  exit 1
}

# ---------------------------------------------------------------------------
# Parsing of command line arguments
#
parseArguments() {
  while getopts "p:h" opt; do
    case $opt in
      p)
        DM_SRC_PATH="$OPTARG"
        ;;
      h)
        usage
        ;;
      \?)
        usage "Invalid option: -$OPTARG"
        ;;
      :)
        usage "Option -$OPTARG requires an argument."
        ;;
    esac
  done
  shift $(($OPTIND - 1))
  REMAINING=("$@")
}

# ---------------------------------------------------------------------------
# Check if command line arguments are valid
#
checkArguments() {
  verbose "Checking arguments"

  checkIfDirExists "Diagnostic manager source directory cannot be found" "$DM_SRC_PATH"
}

adapt-generated-sources()
{
echo "Running clang format DM..."
find $DM_SRC_PATH/src-gen/ -name "*.cc" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/src-gen/ -name "*.cc" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/src-gen/ -name "*.h" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/src-gen/ -name "*.h" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/src-gen/ -name "*.json" -a -type f | xargs -n1 dos2unix

echo " "
echo "Running clang format DiagTestApp..."
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.cc" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.cc" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.h" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.h" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.json" -a -type f | xargs -n1 dos2unix

echo " "
echo "Running clang format DiagTestMediator..."
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.cc" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.cc" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.h" -a -type f | xargs -n1 clang-format -i
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.h" -a -type f | xargs -n1 dos2unix
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.json" -a -type f | xargs -n1 dos2unix


echo " "
echo "Replacing #ifndef..."
find $DM_SRC_PATH/addon/DiagTestApp/src-gen/ -name "*.h" -a -type f | xargs -n1 sed s/' DIAGTESTAPP_'/' ADDON_DIAGTESTAPP_SRC_GEN_'/g -i
find $DM_SRC_PATH/src-gen/ -name "*.h" -a -type f | xargs -n1 sed s/' DIAGNOSTICMANAGER_'/' SRC_GEN_'/g -i
find $DM_SRC_PATH/addon/DiagTestMediator/src-gen/ -name "*.h" -a -type f | xargs -n1 sed s/' DIAGTESTMEDIATOR_'/' ADDON_DIAGTESTMEDIATOR_SRC_GEN_'/g -i
}

# ---------------------------------------------------------------------------
# Main script
#
parseArguments $@
checkArguments
adapt-generated-sources

echo
echo "$0 SUCCESSFUL"
echo

exit 0