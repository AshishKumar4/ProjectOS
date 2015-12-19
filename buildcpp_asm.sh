#!/bin/sh
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
  $MAKE -C $PROJECT build-cpp-nasm
done
