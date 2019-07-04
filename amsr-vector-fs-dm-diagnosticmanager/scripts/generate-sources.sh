#!/bin/bash
set -e
trap onEXIT EXIT

# Default values
ARA_COM_SRC_PATH=""
EXEC_MGR_SRC_PATH=""
DM_SRC_PATH=""
OUTPUT_PATH=""
SIP_VERSION=""
ADDON_SRC_GEN=""

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
  echo "Generate the sources using ARXML files" >&2
  echo >&2
  echo "Options:" >&2
  echo "  -s                    SIP version to use (ex: 20.00.50)." >&2
  echo >&2
  echo "  -c                    Path to the source directory of communication middleware." >&2
  echo >&2
  echo "  -e                    Path to the source directory of execution manager." >&2
  echo >&2
  echo "  -d                    Path to the source directory of diagnostic manager." >&2
  echo >&2
  echo "  -o                    Path where to generate sources (Output directory)." >&2
  echo >&2
  echo "  -h                    Print this help and exit" >&2
  echo >&2
  exit 1
}

# ---------------------------------------------------------------------------
# Parsing of command line arguments
#
parseArguments() {
  while getopts "s:o:d:e:c:h" opt; do
    case $opt in
      s)
        SIP_VERSION="-s $OPTARG"
        ;;
      e)
        EXEC_MGR_SRC_PATH="$OPTARG"
        ;;
      c)
        ARA_COM_SRC_PATH="$OPTARG"
        ;;
      d)
        DM_SRC_PATH="$OPTARG"
        ;;
      o)
        OUTPUT_PATH="$OPTARG"
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

  if [ "$SIP_VERSION" = "" ]; then usage "SIP version is missing (option -s)"; fi;
  if [ "$EXEC_MGR_SRC_PATH" = "" ]; then usage "Path to execution manager is missing (option -e)"; fi;
  if [ "$ARA_COM_SRC_PATH" = "" ]; then usage "Path to communication middleware is missing (option -c)"; fi;
  if [ "$DM_SRC_PATH" = "" ]; then usage "Path to diagnostic manager is missing (option -d)"; fi;
  if [ "$OUTPUT_PATH" = "" ]; then usage "Output folder is not specified (option -o)"; fi;
}

# ---------------------------------------------------------------------------
# Check if command line arguments are valid
#
checkArguments() {
  verbose "Checking arguments"

  checkIfDirExists "Diagnostic manager source directory cannot be found" "$DM_SRC_PATH"
  checkIfDirExists "Execution manager source directory cannot be found" "$EXEC_MGR_SRC_PATH"
  checkIfDirExists "Communication middleware source directory cannot be found" "$ARA_COM_SRC_PATH"
  checkIfDirExists "Output directory does not exist" "$OUTPUT_PATH"
}

# ---------------------------------------------------------------------------
# Function to move the generated data to the right place
#
moveGenData()
{
    local SRC_ORIG=$1
    local TARGET_DEST=$2
    rm -rf ${TARGET_DEST}/*
    cp  -r ${SRC_ORIG}/* ${TARGET_DEST}/
}

# ---------------------------------------------------------------------------
# Function setting ADDON_SRC_GEN variable to the src-gen folder
# of the selected addon.
#
LocationAddonSrcGen()
{
    ADDON_SRC_GEN="${DM_SRC_PATH}/addon/$1/src-gen"
    echo "Location of srg-gen folder of addon $1: ${ADDON_SRC_GEN}"
}

# ---------------------------------------------------------------------------
# Function setting ADDON_ARXML variable to the arxml folder
# of the selected addon.
#
LocationAddonARXML()
{
    ADDON_ARXML="${DM_SRC_PATH}/addon/$1/etc/arxml"
    echo "Location of arxml folder of addon $1: ${ADDON_ARXML}"
}

# ---------------------------------------------------------------------------
# Function genereting sources for one specific arxml folder
#
generateARXML()
{
    $ARA_COM_SRC_PATH/generator/amsrgen_base/run-generators -v $GEN_PATHS $SIP_VERSION -x $1 -o $OUTPUT_PATH
    if [ "$?" != "0" ]; then exit $?; fi
}

generate-sources()
{
    GEN_PATHS=" -g $ARA_COM_SRC_PATH/generator/amsrgen_app_vector/build/libs "
    GEN_PATHS+="-g $ARA_COM_SRC_PATH/generator/amsrgen_someip_vector/build/libs "
    GEN_PATHS+="-g $EXEC_MGR_SRC_PATH/generator/amsrgen_em_vector/build/libs "
    ARXML_PATH_DM="${DM_SRC_PATH}/etc/arxml"

    #Empty current output folder
    rm -rf $OUTPUT_PATH/*

    #genrate sources
    #DM
    generateARXML $ARXML_PATH_DM

    #DiagTestApp
    LocationAddonARXML DiagTestApp
    generateARXML $ADDON_ARXML

    #DiagTestMadiator
    LocationAddonARXML DiagTestMediator
    generateARXML $ADDON_ARXML

    #copy generated sources to src-gen folders.
    #DM
    moveGenData ${OUTPUT_PATH}/DiagnosticManager ${DM_SRC_PATH}/src-gen

    #TestApp
    LocationAddonSrcGen DiagTestApp
    moveGenData ${OUTPUT_PATH}/DiagTestApp ${ADDON_SRC_GEN}

    #TestMediator
    LocationAddonSrcGen DiagTestMediator
    moveGenData ${OUTPUT_PATH}/DiagTestMediator ${ADDON_SRC_GEN}
}

# ---------------------------------------------------------------------------
# Main script
#
parseArguments $@
checkArguments
generate-sources

echo
echo "$0 SUCCESSFUL"
echo

exit 0
