#!/bin/bash
# Usage: ./update-submodules.sh [-c|--clean]
set -e

# Update the submodule and load contents
echo "Updating git submodules..."
git submodule update --init

# remove untracked directories for which no submodule exists
if [ "$1" == "-c" ] || [ "$1" == "--clean" ];
then
  echo "Cleaning all untracked files..."
  git clean -ffd;
fi
echo "done."
